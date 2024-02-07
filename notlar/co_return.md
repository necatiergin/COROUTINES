Bir _coroutine_'in sonunu blirtmek için _co_return_ operatörü kullanılır. _co_return_ C++20 standardı ile dile eklenen bir anahtar sözcüktür. <br>
Bir _coroutine_'in tamamlandığını bildirmesi için üç yol vardır:
1. _coroutine_'in, son bir değer olarak _expr_ ifadesinin değerini döndürmesi için 

```cpp
co_return expr;
```

deyimi kullanabilir.

Bu durumda, derleyici prom ilgili promise nesnesi olmak üzere, kodu

prom.return_value(e);

çağrısına dönüştürür.


2. coroutine'i nihai bir değer olmadan (değersiz)sonlandırmak için 

co_return;

deyimini kullanabiliriz.

Bu durumda, derleyici prom promise nesnesi olmak üzere kodu

prom.return_void();

çağrısına dönüştürür.


3. coroutine'in yürütülmesiyle programın akışı fonksiyonun kodunun sonuna kadar gelir.
Bu durumda, derleyici prom promise nesnesi olmak üzere kodu

prom.return_void();

çağrısına dönüştürür.


Bir coroutine'in tamamlanıp tamamlanmadığını öğrenmek için coroutine_handle nesnesini kullanarak 
coroutine handle sınıfını done isimli fonksiyonunu çağırabiliriz:

bool coroutine handle<>::done();

fonksiyonun true değer döndürmesi corotuin'in çalışmasını tamamladığını fasle değer döndürmesi ise coroutine'in henüz çalışmasını tamamlamadığını gösterir.
