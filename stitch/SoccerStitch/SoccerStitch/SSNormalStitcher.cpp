//
//  SSNormalStitcher.cpp
//  SoccerStitch
//
//  Created by samuel on 26/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include "SSNormalStitcher.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/timelapsers.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"
#include <opencv2/imgproc.hpp>

#define ENABLE_LOG 1
#define LOG(msg) std::cout << msg
#define LOGLN(msg) std::cout << msg << std::endl

using namespace std;
using namespace cv;
using namespace cv::detail;

bool try_cuda = true;
double work_megapix = 0.6;
double seam_megapix = 0.1;
double compose_megapix = -1;

string matcher_type = "homography";
string estimator_type = "homography";
float match_conf = 0.3f;
int range_width = -1;

bool save_graph = true;
string save_graph_to = "/Users/majie/Think/repository/soccercv/cppMatchTemp.txt";
float conf_thresh = 1.f;

//
bool preview = false;
string ba_cost_func = "ray";
string ba_refine_mask = "xxxxx";
bool do_wave_correct = true;
WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;
string warp_type = "spherical";
int expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
string seam_find_type = "gc_color";
int blend_type = Blender::MULTI_BAND;
int timelapse_type = Timelapser::AS_IS;
float blend_strength = 5;
string result_name = "/Users/majie/Think/repository/soccercv/cppdetail.jpg";

//
SSNormalStitcher::SSNormalStitcher(Mat &video1, Mat &video2)
{
    work_scale = 1;
    is_work_scale_set = false;
    
    seam_scale = 1;
    is_seam_scale_set = false;
    
    compose_scale = 1;
    is_compose_scale_set = false;
    
    seam_work_aspect = 1;
    
    vector<Mat> images;
    vector<String> img_names;
    
    Mat image1 = imread("/Users/majie/Think/repository/soccercv/1.jpg");
    images.push_back(image1);
    img_names.push_back("/Users/majie/Think/repository/soccercv/1.jpg");
    
    Mat image2 = imread("/Users/majie/Think/repository/soccercv/2.jpg");
    images.push_back(image2);
    img_names.push_back("/Users/majie/Think/repository/soccercv/2.jpg");
    
    int num_images = static_cast<int>(images.size());
    
    LOGLN("Finding features...");
    
#if ENABLE_LOG
    int64 t = getTickCount();
#endif
    
    //Ptr<FeaturesFinder> finder = makePtr<SurfFeaturesFinder>();
    Ptr<FeaturesFinder> finder = makePtr<OrbFeaturesFinder>();
    
    Mat full_img, img;
    vector<ImageFeatures> features(num_images);
    vector<Size> full_img_sizes(num_images);
    
    for (int i = 0; i < num_images; ++i)
    {
        full_img = images[i];
        full_img_sizes[i] = full_img.size();
        
        if (work_megapix < 0)
        {
            img = full_img;
            work_scale = 1;
            is_work_scale_set = true;
        }
        else
        {
            if (!is_work_scale_set)
            {
                work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
                is_work_scale_set = true;
            }
            resize(full_img, img, Size(), work_scale, work_scale, INTER_CUBIC);
        }
        if (!is_seam_scale_set)
        {
            seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
            seam_work_aspect = seam_scale / work_scale;
            is_seam_scale_set = true;
        }
        
        (*finder)(img, features[i]);
        features[i].img_idx = i;
        LOGLN("Features in image #" << i+1 << ": " << features[i].keypoints.size());
        
        resize(full_img, img, Size(), seam_scale, seam_scale, INTER_CUBIC);
        images[i] = img.clone();
    } //for extract feature
    
    finder->collectGarbage();
    full_img.release();
    img.release();
    LOGLN("Finding features, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
    
    LOG("Pairwise matching");
#if ENABLE_LOG
    t = getTickCount();
#endif
    vector<MatchesInfo> pairwise_matches;
    Ptr<FeaturesMatcher> matcher;
    if (matcher_type == "affine")
        matcher = makePtr<AffineBestOf2NearestMatcher>(false, try_cuda, match_conf);
    else if (range_width==-1)
        matcher = makePtr<BestOf2NearestMatcher>(try_cuda, match_conf);
    else
        matcher = makePtr<BestOf2NearestRangeMatcher>(range_width, try_cuda, match_conf);
    
    (*matcher)(features, pairwise_matches);
    matcher->collectGarbage();
    
    LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
    
    // Check if we should save matches graph
    if (save_graph)
    {
        LOGLN("Saving matches graph...");
        ofstream f(save_graph_to.c_str());
        f << matchesGraphAsString(img_names, pairwise_matches, conf_thresh);
    }
    
    // Leave only images we are sure are from the same panorama
    vector<int> indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);
    vector<Mat> img_subset;
    vector<String> img_names_subset;
    vector<Size> full_img_sizes_subset;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        img_names_subset.push_back(img_names[indices[i]]);
        img_subset.push_back(images[indices[i]]);
        full_img_sizes_subset.push_back(full_img_sizes[indices[i]]);
    }
    
    images = img_subset;
    img_names = img_names_subset;
    full_img_sizes = full_img_sizes_subset;
    
    // Check if we still have enough images
    num_images = static_cast<int>(img_names.size());
    if (num_images < 2)
    {
        LOGLN("Need more images");
        return;
    }
    
    Ptr<Estimator> estimator;
    if (estimator_type == "affine")
        estimator = makePtr<AffineBasedEstimator>();
    else
        estimator = makePtr<HomographyBasedEstimator>();
    
    if (!(*estimator)(features, pairwise_matches, cameras))
    {
        cout << "Homography estimation failed.\n";
        return;
    }
    
    for (size_t i = 0; i < cameras.size(); ++i)
    {
        Mat R;
        cameras[i].R.convertTo(R, CV_32F);
        cameras[i].R = R;
        LOGLN("Initial camera intrinsics #" << indices[i]+1 << ":\nK:\n" << cameras[i].K() << "\nR:\n" << cameras[i].R);
    }
    
    Ptr<detail::BundleAdjusterBase> adjuster;
    if (ba_cost_func == "reproj") adjuster = makePtr<detail::BundleAdjusterReproj>();
    else if (ba_cost_func == "ray") adjuster = makePtr<detail::BundleAdjusterRay>();
    else if (ba_cost_func == "affine") adjuster = makePtr<detail::BundleAdjusterAffinePartial>();
    else if (ba_cost_func == "no") adjuster = makePtr<NoBundleAdjuster>();
    else
    {
        cout << "Unknown bundle adjustment cost function: '" << ba_cost_func << "'.\n";
        return ;
    }
    adjuster->setConfThresh(conf_thresh);
    Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
    if (ba_refine_mask[0] == 'x') refine_mask(0,0) = 1;
    if (ba_refine_mask[1] == 'x') refine_mask(0,1) = 1;
    if (ba_refine_mask[2] == 'x') refine_mask(0,2) = 1;
    if (ba_refine_mask[3] == 'x') refine_mask(1,1) = 1;
    if (ba_refine_mask[4] == 'x') refine_mask(1,2) = 1;
    adjuster->setRefinementMask(refine_mask);
    if (!(*adjuster)(features, pairwise_matches, cameras))
    {
        cout << "Camera parameters adjusting failed.\n";
        return;
    }
    
    // Find median focal length
    
    vector<double> focals;
    for (size_t i = 0; i < cameras.size(); ++i)
    {
        LOGLN("Camera #" << indices[i]+1 << ":\nK:\n" << cameras[i].K() << "\nR:\n" << cameras[i].R);
        focals.push_back(cameras[i].focal);
    }
    
    sort(focals.begin(), focals.end());
    if (focals.size() % 2 == 1)
        warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
    else
        warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;
    
    if (do_wave_correct)
    {
        vector<Mat> rmats;
        for (size_t i = 0; i < cameras.size(); ++i)
            rmats.push_back(cameras[i].R.clone());
        waveCorrect(rmats, wave_correct);
        for (size_t i = 0; i < cameras.size(); ++i)
            cameras[i].R = rmats[i];
    }

    cout<<"\n\n\n*****************init done\n";
    return ;
}

Mat SSNormalStitcher::stitch(Mat &frame1, Mat &frame2, size_t &dst_width, size_t &dst_height)
{
    is_compose_scale_set = false;
    
    Mat result, result_mask;
    
    vector<Mat> images, full_images;
    
    full_images.push_back(frame1);
    full_images.push_back(frame2);
    images.push_back(frame1);
    images.push_back(frame2);
    
    Mat full_img, img;
    int num_images = static_cast<int>(images.size());
    
    vector<CameraParams> thisCameras;
    for (int i = 0; i < num_images; i++)
    {
        thisCameras.push_back(cameras[i]);
    }
    
    vector<Size> full_img_sizes(num_images);
    //init resize & save size
    for (int i = 0; i < num_images; i++)
    {
        full_img_sizes[i] = full_images[i].size();
        resize(images[i], img, Size(), seam_scale, seam_scale, INTER_CUBIC);
        images[i] = img.clone();
    }
    
    //有可能下面是根据上面得到的参数拼接图片了
    
    LOGLN("Warping images (auxiliary)... ");
    
    vector<Point> corners(num_images);
    vector<UMat> masks_warped(num_images);
    vector<UMat> images_warped(num_images);
    vector<Size> sizes(num_images);
    vector<UMat> masks(num_images); //not needed
    
    // Preapre images masks
    for (int i = 0; i < num_images; ++i)
    {
        masks[i].create(images[i].size(), CV_8U); //注意这里的images[]是经过缩放的, 非原图
        masks[i].setTo(Scalar::all(255));
    }
    
    // Warp images and their masks
    
    Ptr<WarperCreator> warper_creator;
    warper_creator = makePtr<cv::SphericalWarper>();
    if (!warper_creator)
    {
        cout << "Can't create the following warper '" << warp_type << "'\n";
        return result;
    }
    
    Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
    
    for (int i = 0; i < num_images; ++i)
    {
        Mat_<float> K;
        thisCameras[i].K().convertTo(K, CV_32F);
        float swa = (float)seam_work_aspect;
        K(0,0) *= swa; K(0,2) *= swa;
        K(1,1) *= swa; K(1,2) *= swa;
        
        corners[i] = warper->warp(images[i], K, thisCameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
        sizes[i] = images_warped[i].size();
        
        warper->warp(masks[i], K, thisCameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
    }
    
    vector<UMat> images_warped_f(num_images);
    for (int i = 0; i < num_images; ++i)
        images_warped[i].convertTo(images_warped_f[i], CV_32F);
    
    Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);
    compensator->feed(corners, images_warped, masks_warped);
    
    Ptr<SeamFinder> seam_finder;
    if (seam_find_type == "no")
        seam_finder = makePtr<detail::NoSeamFinder>();
    else if (seam_find_type == "voronoi")
        seam_finder = makePtr<detail::VoronoiSeamFinder>();
    else if (seam_find_type == "gc_color")
    {
#ifdef HAVE_OPENCV_CUDALEGACY
        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
        else
#endif
            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);
    }
    else if (seam_find_type == "gc_colorgrad")
    {
#ifdef HAVE_OPENCV_CUDALEGACY
        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
        else
#endif
            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
    }
    else if (seam_find_type == "dp_color")
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);
    else if (seam_find_type == "dp_colorgrad")
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
    if (!seam_finder)
    {
        cout << "Can't create the following seam finder '" << seam_find_type << "'\n";
        return result;
    }
    
    seam_finder->find(images_warped_f, corners, masks_warped);
    
    // Release unused memory
    images.clear();
    images_warped.clear();
    images_warped_f.clear();
    masks.clear();
    
    LOGLN("Compositing...");
    Mat img_warped, img_warped_s;
    Mat dilated_mask, seam_mask, mask, mask_warped;
    Ptr<Blender> blender;
    Ptr<Timelapser> timelapser;
    //double compose_seam_aspect = 1;
    double compose_work_aspect = 1;
    
    for (int img_idx = 0; img_idx < num_images; ++img_idx)
    {
        // Read image and resize it if necessary
        full_img = full_images[img_idx];
        if (!is_compose_scale_set)
        {
            if (compose_megapix > 0)
                compose_scale = min(1.0, sqrt(compose_megapix * 1e6 / full_img.size().area()));
            is_compose_scale_set = true;
            cout<<"compose_scale: "<<compose_scale<<endl;
            
            // Compute relative scales
            //compose_seam_aspect = compose_scale / seam_scale;
            compose_work_aspect = compose_scale / work_scale;
            
            // Update warped image scale
            float my_warped_image_scale = warped_image_scale * static_cast<float>(compose_work_aspect);
            //warped_image_scale *= static_cast<float>(compose_work_aspect);
            warper = warper_creator->create(my_warped_image_scale);
            cout<<"warped_image_scale: "<<my_warped_image_scale<<endl;
            
            // Update corners and sizes
            for (int i = 0; i < num_images; ++i)
            {
                // Update intrinsics
                thisCameras[i].focal *= compose_work_aspect;
                thisCameras[i].ppx *= compose_work_aspect;
                thisCameras[i].ppy *= compose_work_aspect;
                cout<<"cameras[i].focal: "<<thisCameras[i].focal<<endl;
                
                // Update corner and size
                Size sz = full_img_sizes[i];
                if (std::abs(compose_scale - 1) > 1e-1)
                {
                    sz.width = cvRound(full_img_sizes[i].width * compose_scale);
                    sz.height = cvRound(full_img_sizes[i].height * compose_scale);
                }
                
                Mat K;
                thisCameras[i].K().convertTo(K, CV_32F);
                Rect roi = warper->warpRoi(sz, K, thisCameras[i].R);
                corners[i] = roi.tl();
                sizes[i] = roi.size();
            }
        }
        if (abs(compose_scale - 1) > 1e-1)
            resize(full_img, img, Size(), compose_scale, compose_scale, INTER_CUBIC);
        else
            img = full_img;
        full_img.release();
        Size img_size = img.size();
        
        Mat K;
        thisCameras[img_idx].K().convertTo(K, CV_32F);
        
        // Warp the current image
        warper->warp(img, K, thisCameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);
        
        // Warp the current image mask
        mask.create(img_size, CV_8U);
        mask.setTo(Scalar::all(255));
        warper->warp(mask, K, thisCameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);
        
        // Compensate exposure
        compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped);
        
        img_warped.convertTo(img_warped_s, CV_16S);
        img_warped.release();
        img.release();
        mask.release();
        
        dilate(masks_warped[img_idx], dilated_mask, Mat());
        resize(dilated_mask, seam_mask, mask_warped.size(), 0, 0, INTER_CUBIC);
        mask_warped = seam_mask & mask_warped;
        
        if (!blender)
        {
            blender = Blender::createDefault(blend_type, try_cuda);
            Size dst_sz = resultRoi(corners, sizes).size();
            cout<<"dst_sz: "<<dst_sz<<endl;
            dst_width = dst_sz.width;
            dst_height = dst_sz.height;
            
            float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
            if (blend_width < 1.f)
                blender = Blender::createDefault(Blender::NO, try_cuda);
            else if (blend_type == Blender::MULTI_BAND)
            {
                MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(blender.get());
                mb->setNumBands(static_cast<int>(ceil(log(blend_width)/log(2.)) - 1.));
                LOGLN("Multi-band blender, number of bands: " << mb->numBands());
            }
            else if (blend_type == Blender::FEATHER)
            {
                FeatherBlender* fb = dynamic_cast<FeatherBlender*>(blender.get());
                fb->setSharpness(1.f/blend_width);
                LOGLN("Feather blender, sharpness: " << fb->sharpness());
            }
            blender->prepare(corners, sizes);
        }
        
        // Blend the current image
        {
            blender->feed(img_warped_s, mask_warped, corners[img_idx]);
        }
    }
    
    blender->blend(result, result_mask);
    //imwrite(result_name, result)=====
    
    return result;
}

Mat SSNormalStitcher::stitch(Mat &frame1, Mat &frame2)
{
    size_t width, height;
    Mat result = this->stitch(frame1, frame2, width, height);
    return result;
}

int SSNormalStitcher::matchMov(const char *file1, int &index1, const char *file2, int &index2)
{
    return 0;
}

void SSNormalStitcher::test(void)
{
#if ENABLE_LOG
    int64 app_start_time = getTickCount();
#endif
    
    vector<Mat> images;
    vector<String> img_names;
    
    Mat image1 = imread("/Users/majie/Think/repository/soccercv/1.jpg");
    images.push_back(image1);
    img_names.push_back("/Users/majie/Think/repository/soccercv/1.jpg");
    
    Mat image2 = imread("/Users/majie/Think/repository/soccercv/2.jpg");
    images.push_back(image2);
    img_names.push_back("/Users/majie/Think/repository/soccercv/2.jpg");
    
    int num_images = static_cast<int>(images.size());
    
    double work_scale = 1, seam_scale = 1, compose_scale = 1;
    bool is_work_scale_set = false, is_seam_scale_set = false, is_compose_scale_set = false;
    
    LOGLN("Finding features...");
    
#if ENABLE_LOG
    int64 t = getTickCount();
#endif
    
    //Ptr<FeaturesFinder> finder = makePtr<SurfFeaturesFinder>();
    Ptr<FeaturesFinder> finder = makePtr<OrbFeaturesFinder>();
    
    Mat full_img, img;
    vector<ImageFeatures> features(num_images);
    vector<Size> full_img_sizes(num_images);
    double seam_work_aspect = 1;
    
    for (int i = 0; i < num_images; ++i)
    {
        full_img = images[i];
        full_img_sizes[i] = full_img.size();

        if (work_megapix < 0)
        {
            img = full_img;
            work_scale = 1;
            is_work_scale_set = true;
        }
        else
        {
            if (!is_work_scale_set)
            {
                work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
                is_work_scale_set = true;
            }
            resize(full_img, img, Size(), work_scale, work_scale, INTER_CUBIC);
        }
        if (!is_seam_scale_set)
        {
            seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
            seam_work_aspect = seam_scale / work_scale;
            is_seam_scale_set = true;
        }
        
        (*finder)(img, features[i]);
        features[i].img_idx = i;
        LOGLN("Features in image #" << i+1 << ": " << features[i].keypoints.size());
        
        resize(full_img, img, Size(), seam_scale, seam_scale, INTER_CUBIC);
        images[i] = img.clone();
    } //for extract feature
    
    finder->collectGarbage();
    full_img.release();
    img.release();
    LOGLN("Finding features, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
    
    LOG("Pairwise matching");
#if ENABLE_LOG
    t = getTickCount();
#endif
    vector<MatchesInfo> pairwise_matches;
    Ptr<FeaturesMatcher> matcher;
    if (matcher_type == "affine")
        matcher = makePtr<AffineBestOf2NearestMatcher>(false, try_cuda, match_conf);
    else if (range_width==-1)
        matcher = makePtr<BestOf2NearestMatcher>(try_cuda, match_conf);
    else
        matcher = makePtr<BestOf2NearestRangeMatcher>(range_width, try_cuda, match_conf);

    (*matcher)(features, pairwise_matches);
    matcher->collectGarbage();

    LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
 
    // Check if we should save matches graph
    if (save_graph)
    {
        LOGLN("Saving matches graph...");
        ofstream f(save_graph_to.c_str());
        f << matchesGraphAsString(img_names, pairwise_matches, conf_thresh);
    }
    
    // Leave only images we are sure are from the same panorama
    vector<int> indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);
    vector<Mat> img_subset;
    vector<String> img_names_subset;
    vector<Size> full_img_sizes_subset;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        img_names_subset.push_back(img_names[indices[i]]);
        img_subset.push_back(images[indices[i]]);
        full_img_sizes_subset.push_back(full_img_sizes[indices[i]]);
    }
    
    images = img_subset;
    img_names = img_names_subset;
    full_img_sizes = full_img_sizes_subset;
    
    // Check if we still have enough images
    num_images = static_cast<int>(img_names.size());
    if (num_images < 2)
    {
        LOGLN("Need more images");
        return;
    }
    
    Ptr<Estimator> estimator;
    if (estimator_type == "affine")
        estimator = makePtr<AffineBasedEstimator>();
    else
        estimator = makePtr<HomographyBasedEstimator>();
    
    vector<CameraParams> cameras;
    if (!(*estimator)(features, pairwise_matches, cameras))
    {
        cout << "Homography estimation failed.\n";
        return;
    }
    
    for (size_t i = 0; i < cameras.size(); ++i)
    {
        Mat R;
        cameras[i].R.convertTo(R, CV_32F);
        cameras[i].R = R;
        LOGLN("Initial camera intrinsics #" << indices[i]+1 << ":\nK:\n" << cameras[i].K() << "\nR:\n" << cameras[i].R);
    }
    
    Ptr<detail::BundleAdjusterBase> adjuster;
    if (ba_cost_func == "reproj") adjuster = makePtr<detail::BundleAdjusterReproj>();
    else if (ba_cost_func == "ray") adjuster = makePtr<detail::BundleAdjusterRay>();
    else if (ba_cost_func == "affine") adjuster = makePtr<detail::BundleAdjusterAffinePartial>();
    else if (ba_cost_func == "no") adjuster = makePtr<NoBundleAdjuster>();
    else
    {
        cout << "Unknown bundle adjustment cost function: '" << ba_cost_func << "'.\n";
        return ;
    }
    adjuster->setConfThresh(conf_thresh);
    Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
    if (ba_refine_mask[0] == 'x') refine_mask(0,0) = 1;
    if (ba_refine_mask[1] == 'x') refine_mask(0,1) = 1;
    if (ba_refine_mask[2] == 'x') refine_mask(0,2) = 1;
    if (ba_refine_mask[3] == 'x') refine_mask(1,1) = 1;
    if (ba_refine_mask[4] == 'x') refine_mask(1,2) = 1;
    adjuster->setRefinementMask(refine_mask);
    if (!(*adjuster)(features, pairwise_matches, cameras))
    {
        cout << "Camera parameters adjusting failed.\n";
        return;
    }
    
    // Find median focal length
    
    vector<double> focals;
    for (size_t i = 0; i < cameras.size(); ++i)
    {
        LOGLN("Camera #" << indices[i]+1 << ":\nK:\n" << cameras[i].K() << "\nR:\n" << cameras[i].R);
        focals.push_back(cameras[i].focal);
    }
    
    sort(focals.begin(), focals.end());
    float warped_image_scale;
    if (focals.size() % 2 == 1)
        warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
    else
        warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;
    
    if (do_wave_correct)
    {
        vector<Mat> rmats;
        for (size_t i = 0; i < cameras.size(); ++i)
            rmats.push_back(cameras[i].R.clone());
        waveCorrect(rmats, wave_correct);
        for (size_t i = 0; i < cameras.size(); ++i)
            cameras[i].R = rmats[i];
    }
    
    //有可能下面是根据上面得到的参数拼接图片了
    
    LOGLN("Warping images (auxiliary)... ");
#if ENABLE_LOG
    t = getTickCount();
#endif
    
    vector<Point> corners(num_images);
    vector<UMat> masks_warped(num_images);
    vector<UMat> images_warped(num_images);
    vector<Size> sizes(num_images);
    vector<UMat> masks(num_images); //not needed
    
    // Preapre images masks
    for (int i = 0; i < num_images; ++i)
    {
        masks[i].create(images[i].size(), CV_8U);
        masks[i].setTo(Scalar::all(255));
    }
    
    // Warp images and their masks
    
    Ptr<WarperCreator> warper_creator;
    warper_creator = makePtr<cv::SphericalWarper>();
    if (!warper_creator)
    {
        cout << "Can't create the following warper '" << warp_type << "'\n";
        return;
    }
    
    Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
    
    for (int i = 0; i < num_images; ++i)
    {
        Mat_<float> K;
        cameras[i].K().convertTo(K, CV_32F);
        float swa = (float)seam_work_aspect;
        K(0,0) *= swa; K(0,2) *= swa;
        K(1,1) *= swa; K(1,2) *= swa;
        
        corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
        sizes[i] = images_warped[i].size();
        
        warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
    }
    
    vector<UMat> images_warped_f(num_images);
    for (int i = 0; i < num_images; ++i)
        images_warped[i].convertTo(images_warped_f[i], CV_32F);
    
    LOGLN("Warping images, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
    
    Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);
    compensator->feed(corners, images_warped, masks_warped);
    
    Ptr<SeamFinder> seam_finder;
    if (seam_find_type == "no")
        seam_finder = makePtr<detail::NoSeamFinder>();
    else if (seam_find_type == "voronoi")
        seam_finder = makePtr<detail::VoronoiSeamFinder>();
    else if (seam_find_type == "gc_color")
    {
#ifdef HAVE_OPENCV_CUDALEGACY
        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR);
        else
#endif
            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR);
    }
    else if (seam_find_type == "gc_colorgrad")
    {
#ifdef HAVE_OPENCV_CUDALEGACY
        if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
            seam_finder = makePtr<detail::GraphCutSeamFinderGpu>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
        else
#endif
            seam_finder = makePtr<detail::GraphCutSeamFinder>(GraphCutSeamFinderBase::COST_COLOR_GRAD);
    }
    else if (seam_find_type == "dp_color")
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR);
    else if (seam_find_type == "dp_colorgrad")
        seam_finder = makePtr<detail::DpSeamFinder>(DpSeamFinder::COLOR_GRAD);
    if (!seam_finder)
    {
        cout << "Can't create the following seam finder '" << seam_find_type << "'\n";
        return;
    }
    
    seam_finder->find(images_warped_f, corners, masks_warped);
    
    // Release unused memory
    images.clear();
    images_warped.clear();
    images_warped_f.clear();
    masks.clear();
    
    LOGLN("Compositing...");
#if ENABLE_LOG
    t = getTickCount();
#endif
    
    Mat img_warped, img_warped_s;
    Mat dilated_mask, seam_mask, mask, mask_warped;
    Ptr<Blender> blender;
    Ptr<Timelapser> timelapser;
    //double compose_seam_aspect = 1;
    double compose_work_aspect = 1;
    
    for (int img_idx = 0; img_idx < num_images; ++img_idx)
    {
        LOGLN("Compositing image #" << indices[img_idx]+1);
        
        // Read image and resize it if necessary
        full_img = imread(img_names[img_idx]);
        if (!is_compose_scale_set)
        {
            if (compose_megapix > 0)
                compose_scale = min(1.0, sqrt(compose_megapix * 1e6 / full_img.size().area()));
            is_compose_scale_set = true;
            
            // Compute relative scales
            //compose_seam_aspect = compose_scale / seam_scale;
            compose_work_aspect = compose_scale / work_scale;
            
            // Update warped image scale
            warped_image_scale *= static_cast<float>(compose_work_aspect);
            warper = warper_creator->create(warped_image_scale);
            
            // Update corners and sizes
            for (int i = 0; i < num_images; ++i)
            {
                // Update intrinsics
                cameras[i].focal *= compose_work_aspect;
                cameras[i].ppx *= compose_work_aspect;
                cameras[i].ppy *= compose_work_aspect;
                
                // Update corner and size
                Size sz = full_img_sizes[i];
                if (std::abs(compose_scale - 1) > 1e-1)
                {
                    sz.width = cvRound(full_img_sizes[i].width * compose_scale);
                    sz.height = cvRound(full_img_sizes[i].height * compose_scale);
                }
                
                Mat K;
                cameras[i].K().convertTo(K, CV_32F);
                Rect roi = warper->warpRoi(sz, K, cameras[i].R);
                corners[i] = roi.tl();
                sizes[i] = roi.size();
            }
        }
        if (abs(compose_scale - 1) > 1e-1)
            resize(full_img, img, Size(), compose_scale, compose_scale, INTER_CUBIC);
        else
            img = full_img;
        full_img.release();
        Size img_size = img.size();
        
        Mat K;
        cameras[img_idx].K().convertTo(K, CV_32F);
        
        // Warp the current image
        warper->warp(img, K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);
        
        // Warp the current image mask
        mask.create(img_size, CV_8U);
        mask.setTo(Scalar::all(255));
        warper->warp(mask, K, cameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);
        
        // Compensate exposure
        compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped);
        
        img_warped.convertTo(img_warped_s, CV_16S);
        img_warped.release();
        img.release();
        mask.release();
        
        dilate(masks_warped[img_idx], dilated_mask, Mat());
        resize(dilated_mask, seam_mask, mask_warped.size(), 0, 0, INTER_CUBIC);
        mask_warped = seam_mask & mask_warped;
        
        if (!blender)
        {
            blender = Blender::createDefault(blend_type, try_cuda);
            Size dst_sz = resultRoi(corners, sizes).size();
            float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
            if (blend_width < 1.f)
                blender = Blender::createDefault(Blender::NO, try_cuda);
            else if (blend_type == Blender::MULTI_BAND)
            {
                MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(blender.get());
                mb->setNumBands(static_cast<int>(ceil(log(blend_width)/log(2.)) - 1.));
                LOGLN("Multi-band blender, number of bands: " << mb->numBands());
            }
            else if (blend_type == Blender::FEATHER)
            {
                FeatherBlender* fb = dynamic_cast<FeatherBlender*>(blender.get());
                fb->setSharpness(1.f/blend_width);
                LOGLN("Feather blender, sharpness: " << fb->sharpness());
            }
            blender->prepare(corners, sizes);
        }

        blender->feed(img_warped_s, mask_warped, corners[img_idx]);
    }
    
    {
        Mat result, result_mask;
        blender->blend(result, result_mask);
        
        LOGLN("Compositing, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
        
        imwrite(result_name, result);
    }
    
    LOGLN("Finished, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");
    
    return ;
}

