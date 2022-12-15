#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>

#pragma region AkGameObjectID
void GameObejctExample()
{
	// GameObject�����Ƚ���ע����������PostEvent��SetPosition�ȡ�
	// ����ʹ��Ԥ����IDע����Ϸ��������ѡ����һ 64 λ�޷������� ID (0 ~ 0x?FFFFFFFFFFFFFFDF)
	// ����ת��Ϊ AkGameObjectID �ĵ��������������ָ��
	const AkGameObjectID GAME_OBJECT_ID_CAR = 100;
	const AkGameObjectID GAME_OBJECT_ID_HUMAN = 200;
	const AkGameObjectID GAME_OBJECT_ID_MARKERS = 300;
	// ĳЩȫ����Ϸ������ܻ��ڳ�ʼ��ʱע�ᣬ���磺DefaultListener
	// �����GameObject������Ϸ��̬ע��/���٣����磺������Ч���ɴݻٵ����������
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_CAR, "Car");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_HUMAN, "Human");
	AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_MARKERS, "MarkerTest");
	// GameObject����ʱ��ʵ�����Ϸ���Ƴ���Ӧ��ע�����ͷ����������Դ��
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_CAR);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_HUMAN);
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_MARKERS);
	// TODO ����ͨ��RegisterGameObj(in_gameObjectID,const char * in_pzzObjName)������дһ��UE�м�����ڲ�����Ƶ�Ĺ���

	// GameObject ������UI��Sound��Trigger��Ԫ�ص�Instance��
	// GameObject������ΪAkGameObjectID(uint64)��������ѡ�� ID����ΧΪ 0 ~ 0x?FFFFFFFFFFFFFFDF����Ϊ�з�������ʱΪ -33���� 
	// ��Ϸ���� ID ��Χ 0x?FFFFFFFFFFFFFFE0?��-32���� 0x?FFFFFFFFFFFFFFFF (-1) �Ǳ������ڲ�ʹ�õġ�
	// transport ��Ϸ��������ƹ���ע�ᣬ����ʹ�õ���Ϸ���� ID �� 0x?FFFFFFFFFFFFFFFE?��-2����

	// ��Ϸ�����������ص� 3D λ�á���Ը����л���������л����ء���Ը��� RTPC �� RTPC ֵ���Լ����ض���Ŀ����Ч������ɫ�����������ߵȣ���Ϊ�ض���Ϸ�������õĶ��ֵ��������������
	// ��������һֱ�洢����Щ��Ϣ��ֱ������Щֵ��ص���Ϸ����ע����
	
	// UnregisterGameObj����GameObject���õĲ��������Ƴ�GameObject�Ĺ���
	AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_ID_MARKERS);
	AK::SoundEngine::UnregisterAllGameObj();
	// TODO ����ͨ���ú���ʵ��UE�е�AudioGM����
}
#pragma endregion


