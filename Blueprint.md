/**
    * 初始化Wwise内存管理器
    * @PARAM uDevicePageSize 设备内存页面尺寸，默认为0即自动检测
    * @PARAM uMemAllocationSizeLimit 默认0为禁用，1调试启用，2 Stomp allocator enabled,3 Stomp allocator and debug enabled. 用户的实现可以通过使用多个非零值来提供不同的功能。
    * @PARAM uVMPageSize 虚拟内存页面大小，默认为0即自动检测
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