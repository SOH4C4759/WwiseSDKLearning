/**
    * ��ʼ��Wwise�ڴ������
    * @PARAM uDevicePageSize �豸�ڴ�ҳ��ߴ磬Ĭ��Ϊ0���Զ����
    * @PARAM uMemAllocationSizeLimit Ĭ��0Ϊ���ã�1�������ã�2 Stomp allocator enabled,3 Stomp allocator and debug enabled. �û���ʵ�ֿ���ͨ��ʹ�ö������ֵ���ṩ��ͬ�Ĺ��ܡ�
    * @PARAM uVMPageSize �����ڴ�ҳ���С��Ĭ��Ϊ0���Զ����
    */
UFUNCTION(BlueprintCallable, Category = "AudioLab|Intergration")
static bool InitMemoryMgr(int uDevicePageSize, int uMemAllocationSizeLimit, int uVMPageSize);
{
    if (GEngine) // Function Call
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Function Called::ChangeAudioDevicePreset!"));
        UE_LOG(LogTemp, Display, TEXT("Called Function:ChangeAudioDevicePreset!"));
    }
}

USTRUCT(BlueprintType)
struct AKAUDIO_API FAkWaapiSubscriptionId
{
	GENERATED_USTRUCT_BODY()
	
	FAkWaapiSubscriptionId() {}
	FAkWaapiSubscriptionId(const uint64_t& SubscribId) : SubscriptionId(SubscribId){}

	mutable uint64 SubscriptionId = 0;
};