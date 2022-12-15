# 普通参数
UFUNCTION(BlueprintCallable)
static void Func1(FString InputStr);

# 多输出引脚
在UE4的C++中，不带Const的引用在蓝图中是默认为输出引脚的，所以可以借此来实现蓝图的多引脚输出
UFUNCTION(BlueprintCallable)
static void Func2(FString& OutputStr);

可以使用Type& Value的方法，使蓝图节点返回大量的参数
UFUNCTION(BlueprintCallable)
static AActor* Func5(AActor *& A1, AActor *& A2, AActor *& A3)

# 带Const的引用
一旦引用带有const，那么他将会变为传入的参数

UFUNCTION(BlueprintCallable)
static void Func3(const FString& ConstInputStr,
                  const FVector& ConstInputVector,
                  const int32& ConstInputInteger
)
{
   // ConstInputStr = TEXT("他不行呀");
   // ConstInputVector = FVector::ZeroVector
}

# C++到蓝图的真正引用

使用UPARAM(ref)来作为修饰
UFUNCTION(BlueprintCallable)
static void Func4(UPARAM(ref) FString& InputStrByRef,
                  UPARAM(ref) FVector& InputVector)
{
   InputStrByRef = TEXT("Hi");
   InputVector = FVector::ZeroVector;
}