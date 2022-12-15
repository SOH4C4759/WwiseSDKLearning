$(WWISESDK)\\include 添加至项目的包含目录下，即调试->调试属性->VC++ 目录->添加至‘包含目录’即可

使用声音引擎的Release配置时定义AK_OPTIMIZED

Release版本中弃用通信代码，如下： #ifndef AK_OPTIMIZED / #endif

在Windows游戏中，使用DLL包装与Wwise相关的内容，需要依赖工程中的定义AKSOUNDENGINE_DLL，与Wwise引擎库静态链接的工程不得定义AKSOUNDENGIEN_DLL