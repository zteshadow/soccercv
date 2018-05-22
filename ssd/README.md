# soccercv



## 1. 搭建Jupyter开发环境

在远端Google Cloud Platform的vm上面跑server, 本地浏览器访问远端服务运行Python代码, 这样开发效率比较高.[参考资料](https://medium.com/@kstseng/%E5%9C%A8-google-cloud-platform-%E4%B8%8A%E4%BD%BF%E7%94%A8-gpu-%E5%92%8C%E5%AE%89%E8%A3%9D%E6%B7%B1%E5%BA%A6%E5%AD%B8%E7%BF%92%E7%9B%B8%E9%97%9C%E5%A5%97%E4%BB%B6-1b118e291015)
1. 登录vm, 运行如下指令配置启动server
```
#生成配置文件
jupyter notebook --generate-config

#编辑配置文件
vim ~/.jupyter/jupyter_notebook_config.py

#在文件尾部添加如下4句配置:
c = get_config()
c.IPKernelApp.pylab = 'inline'
c.NotebookApp.open_browser = False
c.NotebookApp.token = ''

#启动服务器
jupyter notebook
```

2. 默认jupter服务监听端口8888, 因此需要为vm创建防火墙规则, 打开对8888端口的访问权限.然后检查8888端口是否正常监听.
```
netstat -lnp

#看到如下条目标识jupyter服务可以正常访问
...
127.0.0.1:8888           8888
...
```

3. 上传本地ssh密钥[参考资料](http://justcode.ikeepstudying.com/2018/02/linux%EF%BC%9Assh-%E6%97%A0%E5%AF%86%E7%A0%81%E8%BF%9E%E6%8E%A5%E5%88%B0-google-cloud-%E5%AE%9E%E4%BE%8B-filezilla%E8%BF%9E%E6%8E%A5-google-cloud-intellij-idea%E8%BF%9E%E6%8E%A5-google-cloud-goog/)
```
#生成密钥对, 其中[USERNAME]就是vm的登录用户名
ssh-keygen -t rsa -f ~/.ssh/my-ssh-key -C [USERNAME]

#限制访问
chmod 400 ~/.ssh/my-ssh-key

#展示
cat ~/.ssh/my-ssh-key.pub

#格式如下
ssh-rsa [KEY_VALUE] [USERNAME]
[KEY_VALUE] 是生成的公钥值。
[USERNAME] 是实例中您将要为其应用该密钥的用户
```
然后到Google Cloud Platform中的'元数据'页面, 点击SSH密钥, 新建一个item将展示的内容复制粘贴到其中.

4. 建立连接, 访问jupyter notebook
```
ssh -N -f -i ~/.ssh/my-ssh-key -L 8898:localhost:8888 <username>@<IP-address-of-your-GPU-instance>

#my-ssh-key就是刚才生成的密钥对中的私钥
#8898是本地端口, 8888是远端服务器的监听端口
```
连接成功后, 在本地浏览器输入localhost:8898, 就可以打开远端服务了.
