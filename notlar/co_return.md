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
