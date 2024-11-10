#跳一跳项目文档

##操作系统:win11

##c语言环境:MinGW

##编译选项:CMakeLists

##使用的第三方库:SDL2

##游戏方式

###打开:在文件夹中加入dll后直接打开cproject.exe。或者在cmake-build-debug中加入dll用clion运行/启动其中的exe。

###游戏界面中:按p暂停，按r重新开始，esc回到主界面，长按空格蓄力，松开跳跃。

###游戏规则:初始时有一个心，心为0时会结束游戏，每当分数达到4*2^n时你会获得一颗心。游戏中有圆形和矩形两种平台，每当你成功跳到下一个平台时会获得一分，并且视角会重置到蚂蚁。随着分数增加，平台大小的随机数范围不断变小，游戏难度也越高。

###作弊模式隐藏按键:点按h获得一颗心，点按c自动跳跃到下一个平台并得分。

###代码设计:由于屏幕中最多同时出现2-3个平台，因此只需要建立三个平台循环更新，当一个平台离开左边界时在把它刷新在右边界，可以节省很多的空间，相应坏处就是如果32ms内进行两次跳跃操作会导致平台生成异常。