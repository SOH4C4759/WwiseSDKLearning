#include "Wwise_IDs.h"
#include <AK\SoundEngine\Common\AkTypes.h>
#include <AK\SoundEngine\Common\AkSoundEngine.h>

#include <mmeapi.h> // 关于MIDI的Window开发头文件

// SoundEngine初始化后，注册全局回调函数
// 确保在处理消息队列之前注册回调，以确保新发布的MIDI时间在当前帧期间，得到处理

void IntergrationMIDI()
{
	AK::SoundEngine::RegisterGlobalCallback(&MIDICALLBACK, AkGlobalCallbackLocation_PreProcessMessageQueueForRender);
	// FIXME 不允许使用类型
}

#include <AK\SoundEngine\Common\AkMidiTypes.h> // 更多详情见此头文件
// 用于PostMIDIEvent的回调函数
void MIDICallback(bool in_bLastCall)
{
	// 获得SoundEngine的音频设置，这在确定一个音频帧的等效时间时是必要的
	AkAudioSettings audioSettings;
	AK::SoundEngine::GetAudioSettings(audioSettings);

	// 确保此代码对线程来说是安全的
	// 假设需要使用Note On/Off
	AkMIDIPost  aPost[2]; // 继承AkMIDIEvent，该结构体的[2]下标为tNoteOnOff结构体,其中包含AkMidiNoteNo byNote;AkUInt8 byVelocity
	const AkUInt8 byNote = 60;
	const AkUInt8 byChan = 0;

	const AkUInt32 uOnSamples = 0; // 音符将从当前帧的开头开始
	const AkUInt32 uOffSamples = uOnSamples + audioSettings.uNumSamplesPerFrame / 2;// 音符将在一帧当中结束

	AkGameObjectID REGISTER_MIDI_GAME_OBJECT = 100;

	// Note-on
	AkMIDIPost& noteOn = aPost[0];
	noteOn.byType = AK_MIDI_EVENT_TYPE_NOTE_ON;
	noteOn.byChan = byChan;
	noteOn.NoteOnOff.byNote = byNote;
	noteOn.NoteOnOff.byVelocity = 72; // 随机力度
	noteOn.uOffset = uOnSamples;

	// Note-off
	AkMIDIPost& noteOff = aPost[1];
	noteOff.byType = AK_MIDI_EVENT_TYPE_NOTE_OFF;
	noteOff.byChan = byChan;
	noteOff.NoteOnOff.byNote = byNote;
	noteOff.NoteOnOff.byVelocity = 0; // not used for note-off
	noteOff.uOffset = uOffSamples;

	AkUniqueID eventID = AK::SoundEngine::GetIDFromString("MIDIEventName");
	// SoundEngine 从字符串到ID的通用转换器，将给予一下算法对名称进行散列（AK/Tool/Common/AkFNVHash.h)
#include <AK\Tools\Common\AkFNVHash.h>

	AK::SoundEngine::PostMIDIOnEvent(eventID, REGISTER_MIDI_GAME_OBJECT, aPost,2);
	// 对所有在“播放”类型的Action中被指定Event引用的节点执行若干个MIDIEvent，PostMIDIEvent的时间由in_bsoluteOffset决定。
	// 如果为False，MIDIEvent将以uOffset样本从当前帧开始；
	// 如果为True，将以uOffset样本的绝对时间开始。
	// 可通过AK::SoundEngine::GetSampleTick来获取当前绝对时间

	#pragma region AkMIDIPost
	aPost->uOffset;// AKMIDIPost为AkMIDIEvent的子类（继承）
		// uOffset 以采样点为单位为了MIDIEventPost的帧偏移
	// union&struct 
		// union 联合体内可以定义多种不同的数据类型，共享同一段内存
		// union变量所占用的内存长度等于最长的成员的内存长度
		// 【Union相关介绍】https://www.cnblogs.com/jeakeven/p/5113508.html

	AkMIDIEvent testPost;
	// 该结构体中涉及到#pragma pack()的使用。请参考：https://www.jianshu.com/p/d994731f658d

	testPost.Gen.byParam1; 
	testPost.Gen.byParam2;

	testPost.NoteOnOff.byNote; // Note On/Off
	testPost.NoteOnOff.byVelocity; // Velocity

	testPost.Cc.byCc; // CC控制器
	testPost.Cc.byValue; // 该CC控制器的值

	testPost.PitchBend.byValueLsb; // 弯音轮
	testPost.PitchBend.byValueMsb;

	testPost.NoteAftertouch.byNote; // 触后
	testPost.NoteAftertouch.byValue;

	testPost.ChanAftertouch.byValue; // 暂不理解，XX触后？

	testPost.ProgramChange.byProgramNum; // 切换ProgramChange

	testPost.WwiseCmd.uArg; // 见AK_MIDI_WWISE_CMD_ 预处理定义
	testPost.WwiseCmd.uCmd; // 可选的命令语法

	testPost.byChan; // MIDI 通道0~15
	testPost.byType;

	/*
	union
	{
		tGen Gen;
		tCc Cc;
		tNoteOnOff NoteOnOff;
		tPitchBend PitchBend;
		tNoteAftertouch NoteAftertouch;
		tChanAftertouch ChanAftertouch;
		tProgramChange ProgramChange;
		tWwiseCmd WwiseCmd;
	};
	*/
	
	#pragma endregion

	// PostMIDIOnEvent将把MIDIEvent添加到MIDI序列中，在调用RenderAudio之前不会执行任何处理
	
	// 可以通过调用PostMIDIEventf将发送的所有MIDIEvent添加到MIDI序列中
	// 首次调用PostMIDIOnEvent--->PlayingID，将in_playingID设为AK_INVALID_PLAYING_ID
	// 后续使用该playingID将MIDIEvent添加到序列中。
		// 可以通过PostMIDIOnEvent返回的PlayingID确定是否将Event发送至了预定序列
			// 与in_playingID匹配，将Event添加到了预定序列
			// 不匹配，将Event添加到新建序列
			// AK_AK_INVALID_PLAYIND_ID,出错，Event未添加至任何序列
	// 创建新序列的情况
		// 序列（playingID）不存在
		// 使用不同的 Game Object 或 Event 创建了序列。
		// 序列已不存在（已执行所有 MIDI Event 并终止播放所有声音
	// 更新MIDI序列
		// MIDI序列已知，调用一起PostMIDIOnEvent即可发送整个MIDI序列
		// 每帧更新一次MIDI序列，随时调用PostMIDIEvent，但可能在主线程意外调用此函数可能会导致同步问题
		// PostMIDIOnEvent--->RenderAudio之间会处理任意数量的音频帧。
		// 为确保同步，需要组测一个全局回调函数,如下：
		// AK::SoundEngine::RegisterGlobalCallback(&MyCallbackFunction,AkGlobalCallbackLocation_PreProcessMessageQueueForRender)
		// SoundEngine将在每个音频帧上调用整个注册的函数，应用程序可使用回调函数追踪SoundEngine处理的音频帧，以确保MIDI时间正确同步
	// 停止MIDI序列
		// AK::SoundEngine::StopMIDIOnEvent 可停用MIDI序列
		// 可使用无效的输入值充当通配符，将停止所有的MIDI序列
	// TODO 做Wwise MIDI 功能的调试
};