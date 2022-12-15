#include "Wwise_IDs.h" // 引入Soundbank的头文件
#include <SoundEngine\POSIX\AkFilePackageLowLevelIOBlocking.h>
#include <cassert>

#pragma region SoundBank集成
// 初始包Init.bnk必须在其他Soundbank之前加载
bool LoadBankExample()
{
	// SoundBank 名称
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_CAR L"Car.bnk"
#define BANKNAME_HUMAN L"Human.bnk"
#define BANKNAME_MARKERTEST L"MarkerTest.bnk"

// 设置SoundBank路径
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	g_lowLevelIO.SetBasePath(AKTEXT("../../../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Windows/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	// Soundbank同步加载

	AkBankID bankID; // 未使用，SoundBank可以通过文件名卸载。
	AKRESULT eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID);
	assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_CAR, bankID);
	assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_HUMAN, bankID);
	assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_MARKERTEST, bankID);
	assert(eResult == AK_Success);

#pragma region CAKFilePackageLowLevelIOBlocking
	// g_lowLevelIO.AddBasePath(AKTEXT("../../../仅作示范，运行会出错"));
	// g_lowLevelIO.Close()
	// g_lowLevelIO.GetBlockSize()
	// g_lowLevelIO.GetDeviceData();
	// g_lowLevelIO.GetDeviceDesc();
	// g_lowLevelIO.GetFullFilePath();
	// g_lowLevelIO.Init();

	AkUInt32 packageID;
	g_lowLevelIO.LoadFilePackage(AKTEXT(""), packageID);
	// 仅作示范，运行会出错

	// g_lowLevelIO.Open();
	// g_lowLevelIO.OutputSearchedPaths();
	// g_lowLevelIO.Read();
	// g_lowLevelIO.SetPackageFallbackBehavior();
	// g_lowLevelIO.SetUseSubfoldering();
	// g_lowLevelIO.Term();
	// g_lowLevelIO.UnloadAllFilePackages();
	// g_lowLevelIO.UnloadFilePackage();
	// g_lowLevelIO.Write();
	// g_lowLevelIO.~CAkDefaultIOHookBlocking();
	// g_lowLevelIO.~CAkFilePackageLowLevelIO();
	// g_lowLevelIO.~CAkFilePackageLowLevelIOBlocking()
#pragma endregion

// 同样可以使用WAAPI生成Soundbank，即学习使用Api Reference来调用Ak.wwise.ui.commands.execute命令，并想起传递合适的Wwise Authoring Command Identifiers标识

// Soundbank包含:1.事件定义；2.声音结构信息；3.媒体（声音数据、预读声音数据、流播放音频文件的引用）
// Soundbank使用方法：使用ID；使用字符串

	AK::SoundEngine::GetIDFromString("Init.bnk"); // 不区分大小写，将转换为小写后，执行散列函数
	// 除涉及到IO的Ak::SoundEngine::LoadBank()重载外，SDK中所有提供字符串和ID重载的方法都按照这种方式工作

	AkGameObjectID gameObj = 3; // 相关游戏对象ID

	AK::SoundEngine::RegisterGameObj(gameObj);

	// 使用字符串的PostEvent
	AkPlayingID playingID = AK::SoundEngine::PostEvent(L"Play_Sound_01", gameObj); // 事件名称不区分大小写

	// 直接使用ID需要在生成Soundbank时生成头文件Wwise_IDs.h
	// 一旦使用ID，则需要在生成新Soundbank时持续更新头文件，否则会可能发生ID不匹配或编译错误

	// 使用ID的PostEvent
	AkPlayingID playingID_2 = AK::SoundEngine::PostEvent(AK::EVENTS::WM_CM_AMB_WIND_LP, gameObj);

	// ID,String,Wchar三类，同步与异步两种，共6个重载
	// AK::SoundEngine::LoadBank() 的重载之一使用指针和大小，如果向自己执行IO，指针必须在Soundbank卸载前保持有效
	// 直接读取内存的重载版本会解析SoundBank ID，并返回。必须保管好ID，以便用来卸载 SoundBank （使用 AK::SoundEngine::UnloadBank()的ID重载

	// Wwise仅会通过AK::IAKStreamMgr的流管理器执行
	// 可以不沿用此模块，但 Audiokinetic 的默认实现为 I/O 传输和文件系统的底层访问以及 Low-Level IO 模块进一步定义了抽象层。
	// Low-Level IO（底层 IO）模块的接口是 AK::IAkLowLevelIO，并专用于 Audiokinetic 的 AK::IAkStreamMgr 实现。
	// 虽然应用中提供了 Low-Level IO（Low-Level IO）的默认实现，但其目的是让您把它替换成您自己的 Low-Level IO，特别是用于解析文件位置时

	// 使用字符串或ID，AK::IAKLowLevelIO::Open()的版本需要与LoadBank()一致，也取决于生成Soundbank是否Use Soundbank Name选项（会影响IO和文件系统在加载Soundbank的方式）

	// 使用ID版本示例
#include "Wwise_IDs.h" // 通过生成Soundbank时生成头文件
	AK::SoundEngine::LoadBank(AK::BANKS::INIT);

	// 使用字符串示例
	AkBankID testbankID;
#define BANKNAME_INIT L"Init.bnk"
	AK::SoundEngine::LoadBank(BANKNAME_INIT, testbankID);

	// 未选择Use SoundBank Name时，Soundbank在内部通过引用其他Soundbank,从PrepareEvent（）发出额AK::IAKLowLevelIO::Open()的所有调用均使用ID版本，将ID解析为有效的文件描述符时LowLevelIO的责任
	// 在AK::IAkLowLevelIO::Open()的ID版本中，SDK中提供的 Low-Level IO 的默认实现使用ID创建字符串，在末尾加上“.bnk”扩展名，并调用特定于平台的本机FileOpen方法

	// 多语言Soundbank
	// Stream Manager API->创建流->传播语音特征的标志至Low-LevelIO

	// Stream Manager在语言专用的目录中搜索（bIsLanguageSpecific = True)->在通用目录中搜索（bIsLanguageSpecific = false)

	// StreamMedian从Soundbank->IAKLowLevelIO::Open总是使用ID，LowLevelIO的实现使用该ID创建字符串+文件扩展名
	// 复制一份使用此命名方案创建的流播放文件，然后存储在 Generated SoundBanks 目录下，请使用 Project Settings 对话框 SoundBanks 选项卡上的“Copy Streamed Files”选项（post-Generation step)

	// PrepareEvent和PostEvent不可共用，因为两者都会将数据加载到内存中。

#pragma region 显式媒体加载
#include "Wwise_IDs.h"

	AkGameObjectID gameObj = 3;
	AK::SoundEngine::RegisterGameObj(gameObj);

	// 使用ID同步加载Soundbank
		// AK::BANKS::MUSIC 需要加载的SoundBak标识符
		// 返回的SoundBankID
	AkBankID returnBankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(AK::BANKS::MUSIC, returnBankID);

	if (eResult != AK_Success)
	{
		//  SoundBank 加载失败。
		// 处理错误
	}

	AK::SoundEngine::PostEvent(AK::EVENTS::WM_CM_AMB_WIND_LP, gameObj);
#pragma endregion

#pragma region 隐式媒体加载
	// PrepareEvent/PrepareGameSync API可以隐式加载SoundBank中未包含的媒体，为此需要首先通过LoadBank（）显式加载包含Event和GameSync定义的Soundbank
	// 该类Soundbank仅含Event和结构，不含媒体，当Soundbank不包含被引用的媒体时，文件系统必须以松散媒体文件的形式提供该媒体。
	// 在加载Event和GameSync的定义后，必须对它们做”Prepare“操作以便使用
	// 可把声音结构包含在不包含事件定义的 SoundBank 中。然后 Event 所在的 SoundBank 中将包含对含有结构数据的 SoundBank 的引用
	// 在对事件做 Prepare 操作时，被引用 SoundBank 中的结构将与媒体一起隐式加载。
	// 包含的所有结构将从链接的 SoundBank 中加载，而非仅从 Event 所引用的 SoundBank 中加载
	// 为此，把 Event 和结构绑定在同一个 SoundBank 中通常更为实用。
#include "Wwise_IDs.h"

	using namespace AK::SoundEngine; // 注意这里！
	AkGameObjectID gameObj = 3;
	RegisterGameObj(gameObj);

	// 使用ID同步加载Soundbank（只包含事件的定义）
	AkBankID returnBankID;
	AKRESULT eResult = LoadBank(AK::BANKS::SFX, returnBankID); // Event ID

	if (eResult != AK_Success)
	{
		// Soundbank加载失败，处理错误
	}
	// 加载成功，事件的定义及其相关结构已加载
	// 对Event做Prepare处理
	AkUniqueID eventToPrepare = AK::EVENTS::WM_CM_SFX_DASH;

	// 同步地准备Event与否，在此之前，需要LoadBank明确加载包含Event和结构的Soundbank
	// 请求被发送至BankManager之前，解决所有需要的依赖关系，成功播放指定的Event并加载所需的松散媒体文件
	eResult = PrepareEvent(PreparationType::Preparation_Load, &eventToPrepare, 1); // 重载版本：char/id/wchar三种，是否回调两类
	// Input:PreparationType 预备类型,EventName/ID Event的名称或ID,indexEventList Event列表的下标

#pragma region PreparationType
	PreparationType::Preparation_Load; // 将加载所需信息以播放指定的Event
	PreparationType::Preparation_LoadAndDecode; // Vorbis媒体在加载时被解码，未压缩的PCM版本被用于播放
	PreparationType::Preparation_Unload; // 将卸载所需信息以播放指定的Event
#pragma endregion

	if (eResult != AK_Success)
	{
		// 略
	}
	PostEvent(AK::EVENTS::WM_CM_MUS_STT_TFTREMELOIN, gameObj);

#pragma endregion

	// 同步LoadBank为阻塞功能，完成或错误后返回，异步LoadBank立即返回，并在完成请求的操作后，以Cookie作为参数调用回调函数
	// 异步调用时，必须在回调函数中执行错误处理。
	// cookie参数是可选参数，可赋予Null值，SoundEngine将不使用此指针，仅通过回调函数返回。

#pragma region Soundbank回调原型
	typedef void(*AkBankCallbackFunc)
		(
			AkBankID in_bankID,
			void* in_pInMemoryBankPtr,
			AKRESULT in_eLoadResult,
			void* in_pCookie
			);
	// 对于回调函数，必须确保指针的有效性
#pragma endregion

#pragma region 通过内存或文件IO加载Soundbank
// 可以自己从文件中加载SoundBank，然后为相应的LoadBank重载提供指针和大小，也可指定Soundbank标识符，通过Stream Manager加载Soundbank

// 从内存中加载
// 同步，原地
// 同步加载一个Soundbank（内存中，原地，userbank），从内存中加载的Soundbank与原地数据必须使用提供相同内存指针的UnloadBank函数写在，并确保是正确的重载。
// 当通过IO加载时，在缓冲区加载Soundbank，将其地址传递给声音引擎。Soundbank加载请求将被发布，并由BankManager线程，该函数在请求被完全处理后返回
	AKRESULT LoadBankMemoryView
	(
		void* in_pInMemoryBankPtr, // 内存中 已加载Soundbank的指针
		AkUInt32 in_uInMemoryBankSize, // 内存中已加载Soundbank的大小

		// 与异步Soundbank加载卸载请求 一起使用的回调原型。
		// 该回调是由Sounbank线程执行的，在回调函数中的处理事件最小，有太多的处理过程可能会减慢Sounbank的加载过程（引发错误）
		AkBankCallbackFunc in_pfnBankCallback, // 回调函数
		void* in_pCookie, // 回调Cookie
		AkBankID & out_bankID // 返回bankID
	);

	// 异步，原地
	AKRESULT LoadBankMemoryView
	(
		void* in_pInMemoryBankPtr,
		AkUInt32 in_uInMemoryBankSize,
		AkBankCallbackFunc in_pfnBankCallback,
		void* in_pCookie,
		AkBankID & out_bankID
	);

	// SoundEngine不复制内存，在卸载Soundbank之前，内存一直保持有效。具体平台，Soundbank加载涉及的内存指针上可能需要一些对齐限制。
	// 在所有平台上，内存必须与 AK_BANK_PLATFORM_DATA_ALIGNMENT 字节对齐

	// 同步，非原地
	AKRESULT LoadBankMemoryCopy
	(
		void* in_pInMemoryBankPtr,
		AkUInt32 in_uInMemoryBankSize,
		AkBankID & out_bankID
	);

	// 异步，非原地
	// Soundbank 将被复制到声音引擎内部新分配的位置，完成加载操作后立即释放传递的指针
	// LoadBank（）解析随附指针前几个字节中存储的SoundbankID，并返回，保存SoundbankID，用于UnLoadBank（）
	AKRESULT KiadBankMemoryCopy
	(
		void* in_pInMemoryBankPtr,
		AkUInt32 in_uInMemoryBankSize,
		AkBankCallbackFunc in_pfnBankCallback,
		void* in_pCookie,
		AkBankID & out_bandID
	);
	// 自己执行文件IO，并向SoundEngine馈送Pointer指针，必须显式地LoadBank（）,隐式加载地PrepareXXXX命令无法确定Soundbank地数量与内存大小

	// 通过文件IO加载
	// 必须在Sounbank加载前设定Soundbank加载地IO设置
	AK::SoundEngine::SetBankLoadIOSettings(AK_DEFAULT_BANK_THROUGHPUT, AK_DEFAULT_BANK_IO_PRIORITY); // 来自于AK数据类型定义 AkType.h

	// SoundEngine解析Soundbank地元数据，并在SoundEngine地默认池中创建对象
	// 在涉及IO地显式Load Bank中，会从磁盘读取媒体并复制至内存中，IO期间将按照AkInitSettings::uBankReadBufferSize指定的大小分块读取Soundbank
	// 该值越大，读取次数↓，IO期间的内存使用↑
	AkInitSettings bankLoadSetting;
	bankLoadSetting.uBankReadBufferSize = 1024;

#pragma endregion

#pragma region 卸载Soundbank
	// 显式卸载方法：1.使用ID；2.使用字符串；3.同步；4.异步
	AkBankID bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(L"Bank.bnk", bankID);

	if (eResult != AK_Success)
	{
		// 处理错误
	};
	// Soundbank加载成功

	// 对Event进行Prepare操作
	const char* pszEvent = "WM_CM_SFX_WALK";
	eResult = PrepareEvent(Preparation_Load, &pszEvent, 14);
	if (eResult != AK_Success)
	{
		// 处理错误
	}

	// 使用Event和Soundbank数据

	// 解除对Event的Prepara操作
	// 减少与Event相关的结构和媒体的引用次数，当隐式加载的Soundbank的所有对象的引用计数减少至0，会被自动卸载。
	eResult = PrepareEvent(Preparation_Unload, &pszEvent, 14);
	if (eResult != AK_Success)
	{
		// 处理错误
	};

	// 卸载Soundbank
	eResult = AK::SoundEngine::UnloadBank(bankID, NULL);
	if (eResult != AK_Success)
	{
		// 处理错误
	}
	// Preparation_Unload 使用PrepareGamesSyncs（）函数，卸载采用了Prepara操作的Event所加载的媒体。只有选择指定的GameSync后，事件才会播放
	// GameSync没有引用计数，使用Preparation_Unload标志调用一次将立即卸载未被引用的媒体。
	// 猜测：全局RTPC控制Switch容器的方式下，可以利用该机制节省性能资源。

	// : 如果在卸载 SoundBank 时其所引用的声音正在播放，并且 SoundBank 中包含声音的声音结构，则声音将停止播放。
	// 如果 SoundBank 中只包含媒体，但其它已加载的 SoundBank 中有媒体和声音结构，则声音有可能会停止。
	// 这取决于媒体是使用该 SoundBank 中的数据播放还是使用其它 SoundBank 中的数据播放

	// 如果有事件更改过此声音的参数（例如 SetVolume 事件），则此更改信息将被移除。
	// 如果参数已被 RTPC、State 或 Swtich 更改过，则参数将保留在内存中，并将在重新加载 SoundBank 时自动应用

#pragma endregion

#pragma region Soundbank的Prepara
// 该方法可包含所有内容类型，不直接加载媒体文件，而类似于PrepareEvent机制将媒体加载内存中
// PreparaBank()时，会查看媒体在内存中是否已经存在，可以避免媒体重复，从而将内存占用保持最低水平。（优化方式+1）
	PrepareBank(Preparation_Load, testbankID, AkBankContent_All);
	// 如果特定Event的媒体不在Soundbank中，在此之后调用PrepareEvent（）从松散文件中加载

	// 仅加载Event和结构元数据
	PrepareBank(Preparation_Load, testbankID, AkBankContent_StructureOnly)
		;
	// PrepareEvent()必须把媒体当作磁盘中的松散文件进行访问，无法读取包含在Soundbank中的文件
	// 只有打算稍后使用其它加载机制加载 SoundBank 时，AkBankContent_StructureOnly 才有用。
	// 在使用 PrepareEvent() 分别加载媒体的大多数场景中，媒体不应包含在 SoundBank
	// AkBankContent_StructureOnly 标志将产生与 AkBankContent_All 相同的结果。
	// AkBankContent_StructureOnly 标志可能有用的一个场合是实现多个加载的配置。
	// 游戏可有使用 PrepareEvent() 按需加载松散文件的“工具模式”，以及使用 LoadBank() 整体加载同一 SoundBank 的“游戏模式”。
	// 如果需要，可通过 API 同步或异步调用 PrepareBank()。
	// 然而，建议不要对同一 SoundBank 同时使用 AkBankContent_All 和AkBankContent_StructureOnly ，因为媒体一旦使用 AkBankContent_All 进行加载，卸载时将释放所有内容，包括事件、结构和媒体。

#pragma endregion

#pragma region 声音库的操作
/*
ClearBanks();
// 所有声音停止播放
// 所有Soundbank军被卸载，包括Initial.bnk（所有State将被清空，属于初始包）
// 猜测：那么gamesync是否还保存在内存中
ClearPreparedEvents();// ClearBanks会内部调用ClearPreparedEvents，将取消到目前为止已做了 Prepare 操作的所有事件
Term(); // 为了停止SoundEngine，不必ClearBanks
*/
// 第二次显式加载某个Bank时，会导致加载错误
// Wwise 声音引擎可让相同的事件、声音结构或媒体存在于两个或两个以上的 SoundBank 中，并同时全部加载
// 对于不支持媒体重新定位的XMA和OpusNX，如果正在播放几个库共同的声音，而正在读取该声音的SoundBank被卸载，这个声音的播放将停止
// 在使用 PrepareEvent 时，如果多个不同事件需要加载同一媒体内容，则内存中不会出现重复加载该媒体复制品的情况

// 由于在使用 LoadBank 时 SoundBank 作为实体加载，因此结合 PrepareEvent 使用 LoadBank 来加载同一媒体内容可能导致媒体重复。（Bug可能）
#pragma endregion
#pragma endregion

#pragma region SoundBank管理示例
	// 方案因素：内存占用，IO，集成的便利度
	// PreparaEvent&LoadBank不可共用

	#pragma region 一体化Soundbank
	// 一体化Soundbank
	// 适用于：游戏中音频素材数量有限；游戏有大量的内存空间

	/*
	*
	优点：
		•从声音设计师的角度看，用这个方法来维护 SoundBank 内容最容易。
		•更改 SoundBank 的内容时无需重新编译游戏，因为游戏总是加载同一 SoundBank 。
		•游戏中没有复杂的 SoundBank 加载和卸载操作。
		•无需在游戏中跟踪哪些声音可用或不可用。

	缺点：
		•内存使用效率低，整个游戏的所有事件、结构和内存媒体一直保持加载状态。

		技巧 : 虽然此方法可以快速、轻松地把音频集成到游戏中去，但是不要等到项目结束时再换方法来改进内存占用。
	*/
	#pragma endregion 
	
	#pragma region 多个完整Soundbank
	// 多个完整Soundbank，适用于单人游戏
	AkBankID bankID;
	AKRESULT eResult = LoadBank(L"Init.bnk", bankID);
	if (eResult == AK_Success)
	{
		eResult = LoadBank(L"CommonEvents.bnk",bankID);
	}
	// 根据玩家位置或关卡加载Soundbank
	eResult = AK::SoundEngine::LoadBank(L"Level_1.bnk", bankID);
	// ...
	/*
	优点：
		•可能比“一体化” SoundBank 技术需要的内存少得多。
		•非常容易集成到游戏中。

	缺点：
		•不太适合在线游戏或者基于事件的游戏，在这些游戏中，音频需求不仅仅由主人公位置等简单事实来驱动。
		•可能造成内存中重复加载某些音频源，由于不同 SoundBank 中可能会包含重复数据。
		•可能造成 SoundBank 的磁盘总空间增加，因为不同的 SoundBank 可能有相似的内容。
	*/

	// 微管理媒体
	// 适用于：游戏有大量的媒体素材要载入内存；声音设计师难以事先预测媒体需求；项目使用切换开关和状态来确定具体事件要播放的声音；声音不易拆分。
	/*
	1.创建一个叫“EventBank”的 SoundBank ，并加载到 SoundBank Editor。
	2.把“Play_Footstep”事件拖到 SoundBank Editor 的 Add 选项卡中。
	3.取消选择此事件的“Media”复选框，但保留选中 Events 和 Structures 复选框。
	4.创建另外三个 SoundBank，每种纹理对应一个 SoundBank。
	5.将与此纹理相关的随机容器拖入各个 SoundBank 中。 
	6.取消选择三个纹理 SoundBank 中每个 SoundBank 的“Events”和“Structures”复选框，只保留选中 Media（媒体）类型。
	7.生成 SoundBank，然后把生成的 SoundBank 文件夹复制到游戏应用程序中。
	一个 SoundBank 中包含与要播放的音频相关的事件和结构数据，而其它三个 SoundBank 中仅包含与特定地面纹理相关的媒体。
	*/

	AkBankID    bankID; // 此示例中未使用。
	AKRESULT eResult = AK::SoundEngine::LoadBank(L"Init.bnk", bankID);
	if (eResult == AK_Success)
	{
		eResult = AK::SoundEngine::LoadBank(L"EventBank.bnk", bankID);
	}
	if (eResult == AK_Success)
	{
		eResult = AK::SoundEngine::LoadBank(L"Common_Footstep_bank.bnk", bankID);
	}
	eResult = AK::SoundEngine::UnloadBank(L"Winter_Footstep_bank.bnk", NULL);
	eResult = AK::SoundEngine::UnloadBank(L"Desert_Footstep_bank.bnk", NULL);
	// 分别加载声音，可以考虑使用PrepareEvent API，PinEventInStreamCache API 或 SetMedia API

	AkUniqueID in_EventID;
	AkPriority in_uActivePriority;
	AkPriority in_uInactivePriority;
	AK::SoundEngine::PinEventInStreamCache(in_EventID, in_uActivePriority, in_uInactivePriority);
	// 开始将Event引用的流媒体文件的第一部分流入Cache buffer，当没有其他流需要可用带宽时，Cache Stream将被服务
	// 文件将保持缓存，直到UnPinEventInStreamCache被调用，或更高优先级的pinned文件需要控件并且uMaxCachePinnedBytes限制被超过
	// 文件Pinned在缓存中的数据量由Prefetch Size决定，其通过创作工具定义并储存在Sounbank中。
	// 可以通过LowLevel IO覆盖存储在Soundbank中的Prefetch Size.详情见Ak::StreamMgr::IAkFileLocationResolver::Open()与AkFileSystemFlags
	// 如果这个函数在同一事件中被多次调用，那么缓存流的优先级会被更新。
	// 优先级是以文件为单位传递给流管理器的，如果另一个事件被Pinned在缓存中，引用了相同的文件，但优先级不同，那么第一个优先级将以最新的值更新。
	// 如果事件引用的文件是根据状态组选择的（通过开关容器），所有状态下的文件都将被缓存。那些处于当前活动状态的文件将以活动优先级被缓存，而所有其他文件将以非活动优先级被缓存。
	// in_uInactivePriority 仅与引用分配给状态组的开关容器的事件有关。这个参数对于所有其他事件都是忽略的，包括只引用被分配到开关组的开关容器的事件。
	// 基于开关组选择的文件，每个游戏对象都有不同的开关值，并且都被引脚到缓存系统有效地认为是激活的。

	AkSourceSettings sourceSettings;
	AkUniqueID numSourceSettings;
	// AK::SoundEngine::SetMedia(); 
	// 表示特定的Media ID在内存中的位置
	/*
	*
	•是优化内存利用率的最佳选择。
	•您可以全面控制在游戏的什么地方加载哪个媒体。

	缺点：
		•声音设计师和游戏程序员需要大量的沟通，共同决定要加载什么 SoundBank 以及何时加载。
	*/
	#pragma endregion
	
	#pragma region PreparaEvent
	// PreparaEvent
	// 适用于：媒体需要极小的粒度级别，保持较低的内存占用；无需拆分多个Soundbank
	// 在调用 PrepareEvent 功能时，系统分析事件，确保与此事件相关的所有结构和媒体都载入了内存。
	// 否则，系统将自动从磁盘中传输缺少的信息。在显式撤消 Prepare 之前，事件会一直处于准备就绪状态
	/*
	1.创建名为“Events”的 SoundBank，并加载到 SoundBank Editor 中。
	2.把工程中的一些事件添加到“Events” SoundBank 中，或仅添加事件工作单元。
	3.取消选择“Media”复选框，保留选中“Events”和“Structures”复选框。
	4.生成 SoundBank。当生成的 SoundBank 引用了若干件松散媒体时，您将收到警告消息：未在任何 SoundBank 中找到 SoundBank <bank> 中的 <structure> 所引用的媒体。您可以右键单击此消息，把它添加到忽略列表中。
	5.在默认情况下，将使用 CopyStreamedFiles 工具把 SoundBank 所引用的松散媒体复制到 SoundBank 的同一输出目录中。
	6.把生成的 SoundBank 文件夹（随松散媒体一起）复制到游戏应用程序中。
	单一 SoundBank 中所包含的结构数据不可在运行时拆分。因此，如果使用 AK::SoundEngine::PrepareEvent，并且需要另一 SoundBank 中的结构数据，则将一次性加载该 SoundBank 中的所有结构。为此，您可能需要把工程中的结构内容拆分为多个 SoundBank，从而最大限度地避免把不必要的信息载入内存中。
	*/

	// 初始化SoundEngine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// 设置所需设置
	

	AKRESULT eResult = AK::SoundEngine::Init(&initSettings, &platformInitSettings);
	if (eResult != AK_Success)
	{
		// Handle error
	}

	// 加载初始化 SoundBank 和事件/结构 SoundBank 。
	AkBankID    bankID; // 此示例中未使用。
	AKRESULT eResult = AK::SoundEngine::LoadBank(L"Init.bnk", bankID);
	if (eResult == AK_Success)
	{
		eResult = AK::SoundEngine::LoadBank(L"Events.bnk", bankID);
	}

	// 然后在代码中的各个位置：

	const char* pEventsNameArray[1] = { "My_Event_Name" };

	// Prepare 事件：
	eResult = AK::SoundEngine::PrepareEvent(Preparation_Load, pEventsNameArray, 1); // 1 is the array size

	// 撤消 Prepare 事件：
	eResult = AK::SoundEngine::PrepareEvent(Preparation_Unload, pEventsNameArray, 1); // 1 is the array size
	// 
	/*
		•SoundBank 生成过程简单。
		•媒体的粒度级别很小。
		•保持较低的总体内存占用。
		•易于实现自动化。

	缺点：
		•逐一加载媒体素材，可能导致磁盘上的读取和寻址次数增加。
		•对总体内存占用量的控制减弱。
		•不适合互动音乐。
	*/
	#pragma endregion

	#pragma region PrepareEvent&GameSync
	/*
	•媒体需要较小的粒度级别以保持较低的内存占用。
	•无需担心把媒体素材拆分到多个 SoundBank 中。
	•工程中有事件根据切换开关或状态播放不同的声音。
	•工程中有互动音乐需要根据切换开关或状态来播放。
	*/
	// 初始化声音引擎。

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// 设置所需的设置。


	// 设置 PrepareEvent 相关设置。

	////////////////////////////////////////////////////////////////
	// bEnableGameSyncPreparation 标志设为 true，以激活
	// 准备游戏同步器机制。当设置为 false 时，与所有游戏 
	// 同步器相关的媒体将加载，并且无需 
	// 调用 AK::SoundEngine:PrepareGameSyncs。
	//
	// 当设置为 true 时，不会加载与游戏同步器相关的媒体，
	// 除非游戏同步器已通过调用 AK::SoundEngine:PrepareGameSyncs 被激活
	////////////////////////////////////////////////////////////////
		initSettings.bEnableGameSyncPreparation = true;

	AKRESULT eResult = AK::SoundEngine::Init(&initSettings, &platformInitSettings);
	if (eResult != AK_Success)
	{
		// 处理错误。
	}

	// 加载初始化 Bank 和事件/结构 SoundBank 。
	AkBankID    bankID; // 此示例中未使用。
	AKRESULT eResult = AK::SoundEngine::LoadBank(L"Init.bnk", bankID);
	if (eResult == AK_Success)
	{
		eResult = AK::SoundEngine::LoadBank(L"Events.bnk", bankID);
	}

	// ... 在此时， 
	// 两个事件已加载但尚未 Prepare。当前没有加载媒体。

	const char* pNameArray[1];

	// Prepare 主人公脚步事件。
	pNameArray[0] = "Play_Maincharacter_FootSteps";
	eResult = AK::SoundEngine::PrepareEvent(Preparation_Load, pNameArray, 1); // 1 是数组大小  
	// ... 在此时， 
	// 已 Prepare 一个事件，但尚未加载任何媒体。

	// 从现在起，游戏中始终可用 concrete。
	pNameArray[0] = "Concrete";
	eResult = AK::SoundEngine::PrepareGameSyncs(Preparation_Load, in_eType, "GroundTexture", pNameArray, 1);

	// ... 在此时，
	// 已加载 3 个声音：Sound_Concrete_main_1、Sound_Concrete_main_2 和 Sound_Concrete_main_3。

	// 现在，假设主人公进入一片雪地。
	pNameArray[0] = "Snow";
	eResult = AK::SoundEngine::PrepareGameSyncs(Preparation_Load, in_eType, "GroundTexture", pNameArray, 1);

	// ... 在此时，
	// 又加载了 3 个声音：Sound_Snow_main_1、Sound_Snow_main_2 和 Sound_Snow_main_3


	// 然后假设怪兽突然在此时出现。
	pNameArray[0] = "Play_Monster_Footsteps";
	eResult = AK::SoundEngine::PrepareEvent(Preparation_Load, pEventsNameArray, 1); // 1 是数组大小

	// ... 在此时，
	// 又加载了 6 个声音（ Sound_Concrete_Monster_1.2.3 和 Sound_Snow_Monster_1.2.3）

	// 现在我们的玩家决定逃离怪兽，而怪兽在玩家身后紧追不舍。
	// 怪兽和玩家一直奔跑，来到没有积雪的地方。

	pNameArray[0] = "Snow";
	eResult = AK::SoundEngine::PrepareGameSyncs(Preparation_Unload, in_eType, "GroundTexture", pNameArray, 1);

	// ... 在此时，
	// 与积雪相关的 6 个声音（Sound_Snow_Monster_1.2.3 和 Sound_Snow_main_1.2.3）已从内存中卸载。

	/*
		•SoundBank 生成过程简单。
		•媒体的粒度级别很小。
		•保持较低的总体内存占用。
		•易于实现自动化。
		•只加载有用的媒体。
	缺点：
		•逐一加载媒体素材，可能导致磁盘上的读取和寻址次数增加。
		•对内存总体占用量的控制减弱。
		•在 Prepare 大量需要加载新数据的事件时，激活游戏同步器可能导致流播放带宽升高。
	*/
	#pragma endregion



#pragma endregion
