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

Bir _coroutine_'in tamamlanıp tamamlanmadığını öğrenmek için _coroutine_handle_ nesnesini kullanarak std::coroutine_handle sınıfının `done` isimli fonksiyonunu çağırabiliriz:

```cpp
bool coroutine handle<>::done();
```
Fonksiyonun _true_ değer döndürmesi _coroutin_'in çalışmasını tamamladığını _false_ değer döndürmesi ise _coroutine_'in henüz çalışmasını tamamlamadığını gösterir.
