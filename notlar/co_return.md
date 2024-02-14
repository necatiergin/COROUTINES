Bir _coroutine_'in sonunu blirtmek için _co_return_ operatörü kullanılır. _co_return_ C++20 standardı ile dile eklenen bir anahtar sözcüktür. <br>
Bir _coroutine_'in tamamlandığını bildirmesi için üç yol vardır:
1. _coroutine_'in, son bir değer olarak _expr_ ifadesinin değerini döndürmesi için 

```cpp
co_return expr;
```

deyimi kullanabilir.

Bu durumda, derleyici `prom` ilgili _promise_ nesnesi olmak üzere, kodu

```cpp
prom.return_value(e);
```
çağrısına dönüştürür.

2. _coroutine_'i son bir değer olmadan (değersiz olarak) sonlandırmak için 

```cpp
co_return;
```

deyimi kullananılabilir.

Bu durumda, derleyici `prom` _promise_ nesnesi olmak üzere kodu

```cpp
prom.return_void();
```
çağrısına dönüştürür.

3. _coroutine_'in yürütülmesiyle programın akışı fonksiyonun kodunun sonuna kadar gelir. Bu durumda, derleyici prom promise nesnesi olmak üzere koda

```
prom.return_void();
```
çağrısını ekler.<br>
_co_return_ hakkında dikkat edilmesi gereken önemli bir nokta var: _co_return_ deyimi kullanılmadığı sürece `return_void()` fonksiyonunun tanımlanmamış olması bir sorun oluşturmaz. Ancak _co_return_ deyimi kullanılmışsa ve _promise_ sınıfının _return_void_ ya da _return_value_ fonksiyonları yok ise olmayan fonksiyon sentaks hatası oluşturur. 
Eğer programın akışı bir _coroutine_ fonksiyonun sonuna kadar gelirse ve _promise_type_ sınıfının _return_void()_ fonksiyonu yok ise bu durum tanımsız davranıştır. 

Bir _coroutine_'in tamamlanıp tamamlanmadığını öğrenmek için _coroutine_handle_ nesnesini kullanarak std::coroutine_handle sınıfının `done` isimli fonksiyonunu çağırabiliriz:

```cpp
bool coroutine handle<>::done();
```
Fonksiyonun _true_ değer döndürmesi _coroutin_'in çalışmasını tamamladığını _false_ değer döndürmesi ise _coroutine_'in henüz çalışmasını tamamlamadığını gösterir.

_co_return_ ile ilgili dikkat edilmesi gereken diğer bir nokta daha var:
hem _promise_type::return_void()_ hem de _promise_type::return_value()_ _void_ fonksiyonlardır. Yani bir değer döndürmezler. Yani diğer fonksiyonlardan farklı olarak bu fonksiyonların geri dönüş değeri _awaitable_ türlerinden değildir. <br>
Peki bir _coroutine_'in sonunda ne yapılır? 
Derleyici _coroutine_ durumunu güncellemeli ve _coroutine_'i son bir kez durdurmalı mıdır, böylece _co_return_ yürüt sonra bile ana işlevdeki kod promise nesnesine erişebilir ve _coroutine_handle_'ı mantıklı bir şekilde kullanabilir mi?
Yoksa bir _coroutine_'den dönüldüğünde _coroutine_handle::destroy()_ fonksiyonuna yapılan örtülü bir çağrı gibi _coroutine frame_'i otomatik olarak yok mu etmelidir?
Bu soru _promise_type_'ın _final_suspend_ fonksiyonu ile çözülür. 
C++ standardı, bir _coroutine_'in fonksiyon gövdesinin aşağıdaki gibi sarmalandığını söyler _(pseudo code)_

```cpp
{
	promise-type promise promise-constructor-arguments ;
	try {
		co_await promise.initial_suspend() ;
		function-body
	} catch ( ... ) {
	if (!initial-await-resume-called)
		throw ;
		promise.unhandled_exception() ;
	}
	final-suspend :
	co_await promise.final_suspend() ;
}
// "The coroutine state is destroyed when control flows // off the end of the coroutine"
```
Bir _coroutine_ _return_ ettiğinde, örtülü olarak çağrılan _promise.final_suspend()_ işlevinin geri dönüş değeri _co_await_ operatörünün operandı olur.<br>
Eğer _final_suspend_ işlevi gerçekten _coroutine_'i durdurursa, _coroutine state_ son bir kez güncellenir ve geçerli kalır. _coroutine dışındaki_ kod, _coroutine handle_'ın _destroy()_ fonksiyonunu çağırarak _coroutine_ nesnesini ı etmekten sorumlu olur. Eğer _final_suspend_ _coroutin_'i_ suspend_ etmez iae, _coroutine state_ otomatik olarak yok edilecektir.<br>
Eğer _coroutine_ durumuna bir daha dokunulmayacak ise (belki de _coroutine_ _co_return_'den önce global bir değişkeni güncellediği ve/veya bir semaforu serbest bıraktığı içindir ve sizin için önemli olan tek şey budur), o zaman durumu son bir kez kaydetmek için gereksiz maliyete ve _coroutine_ durumunu manuel olarak _deallocate_ etmek için  endişelenmeye gerek yoktur. Bu nedenle _final_suspend()_ fonksiyonunun _std::suspend_never_ döndürmesi sağlanabilir.
<br>Öte yandan, bir _coroutine_ _return_ ettikten sonra _coroutine handle_'ına ya da _promise_ nesnesine erişmeniz gerekiyorsa, _final_suspend()_ işlevinin _std::suspend_always_ (veya _coroutine_'i askıya alan başka bir _awaitable_ nesne) döndürmesi gerekir.
