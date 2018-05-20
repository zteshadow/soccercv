#
# used to gen pascalVOC image set, train&validate, train, validate, test
# 用来生成测试数据集, 用extract_image提取视频图片, 用labelImage标注图片, 用这个脚本
# 生成测试数据集(train&validate = train + validate), test
#
# ==============================================================================
import os, sys
import random  

def show_usage():
    #there must 'Annatations' and 'JPEGSImages' dir in 'dir'
    #this script just gen 'ImageSets\Main' for dir
    print('usage:\n\tpython gen_pascalVOC \'dir\'')

def gen(home):
    xmlfilepath = home + 'Annotations'
    total_xml = os.listdir(xmlfilepath)
    num = len(total_xml)
    if num <= 0:
        print('There is no xml in %s' % xmlfilepath)
        exit(1)
    
    txtsavepath = home + 'ImageSets/Main'
    if not os.path.isdir(home + 'ImageSets'):
        os.mkdir(home + 'ImageSets')
    if not os.path.isdir(txtsavepath):
        os.mkdir(txtsavepath)

    trainval_percent = 0.66
    train_percent = 0.5
    
    list = range(num)
    tv = int(num * trainval_percent)  
    tr = int(tv * train_percent)  
    trainval = random.sample(list, tv)
    train = random.sample(trainval, tr)  

    ftrainval = open(txtsavepath + '/trainval.txt', 'w')  
    ftest = open(txtsavepath + '/test.txt', 'w')  
    ftrain = open(txtsavepath + '/train.txt', 'w')  
    fval = open(txtsavepath + '/val.txt', 'w')  
    
    for i  in list:
        name = total_xml[i][:-4]+'\n'
        if i in trainval:  
            ftrainval.write(name)  
            if i in train:  
                ftrain.write(name)  
            else:  
                fval.write(name)  
        else:  
            ftest.write(name)  
    
    ftrainval.close()  
    ftrain.close()  
    fval.close()  
    ftest .close()

if __name__=='__main__':
    if len(sys.argv) != 2:
        show_usage()
    else:
        dir = sys.argv[1]
        gen(dir)
