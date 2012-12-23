关于目录结构：
firstlight目录是根目录：
doc -> 引擎相关文档
src -> 引擎所有的头文件和实现文件
  -- externs ->源代码形式的第三方lib
libs -> 运行引擎依赖的外部库和头文件，静态库或动态库形式
tools -> 引擎使用到的工具库（游戏不一定用）
projects -> 各平台工程
tests -> 测试工程和代码，直接depend firstlight工程
sdk -> 引擎发布目录，包括头文件，各平台以及配置的lib，工具的win32 exe

ps: 以后开发的编辑器等工具以及sample的目录要放到firstlight平级，作为SDK的使用者