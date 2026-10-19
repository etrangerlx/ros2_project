# ROS2入门21讲 · 古月

- 课程视频：[https://class.guyuehome.com/detail/p_628f4288e4b01c509ab5bc7a/6](https://class.guyuehome.com/detail/p_628f4288e4b01c509ab5bc7a/6)
- 图文教程：[https://book.guyuehome.com/](https://book.guyuehome.com/)
- 课程问答：[https://www.guyuehome.com/Bubble/circleDetail/id/90/](https://www.guyuehome.com/Bubble/circleDetail/id/90/)
- 博客泡圈：[https://www.guyuehome.com/](https://www.guyuehome.com/)


# ros2 容器
## 镜像获取
```
sudo docker pull ros:humble
```
## 启动容器
```
sudo docker run -itd --name ros2_humble \
  --network host \
  -v ~/workspaces/ros2_ws:/root/ros2_ws \
  -w /root/ros2_ws \
  ros:humble
```
# ssh服务
## ssh服务安装
```
apt install -y openssh-server
mkdir -p /run/sshd
passwd root

sed -i 's/^#*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config
sed -i 's/^#*PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config
/usr/sbin/sshd
```
## 网络服务
```
apt install -y iproute2
~/ros2_ws# ss -lntp | grep ':22'
LISTEN 0      128          0.0.0.0:22         0.0.0.0:*    users:(("sshd",pid=1208,fd=3))
LISTEN 0      128             [::]:22            [::]:*    users:(("sshd",pid=1208,fd=4))

```

## ros2 指令

