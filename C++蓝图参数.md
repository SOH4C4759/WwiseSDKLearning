# ��ͨ����
UFUNCTION(BlueprintCallable)
static void Func1(FString InputStr);

# ���������
��UE4��C++�У�����Const����������ͼ����Ĭ��Ϊ������ŵģ����Կ��Խ����ʵ����ͼ�Ķ��������
UFUNCTION(BlueprintCallable)
static void Func2(FString& OutputStr);

����ʹ��Type& Value�ķ�����ʹ��ͼ�ڵ㷵�ش����Ĳ���
UFUNCTION(BlueprintCallable)
static AActor* Func5(AActor *& A1, AActor *& A2, AActor *& A3)

# ��Const������
һ�����ô���const����ô�������Ϊ����Ĳ���

UFUNCTION(BlueprintCallable)
static void Func3(const FString& ConstInputStr,
                  const FVector& ConstInputVector,
                  const int32& ConstInputInteger
)
{
   // ConstInputStr = TEXT("������ѽ");
   // ConstInputVector = FVector::ZeroVector
}

# C++����ͼ����������

ʹ��UPARAM(ref)����Ϊ����
UFUNCTION(BlueprintCallable)
static void Func4(UPARAM(ref) FString& InputStrByRef,
                  UPARAM(ref) FVector& InputVector)
{
   InputStrByRef = TEXT("Hi");
   InputVector = FVector::ZeroVector;
}