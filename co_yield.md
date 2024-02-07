_co_yield_ operatörünü kullanarak bir _coroutine_'in kendisini çağıran koda değer ya da değerler döndürmesini sağlayabiliriz:

co_yield C++20 ile eklenen bir anahtar sözcük.

```cpp
co_yield expr;
```

biçiminde kullanılıyor. Yukarıdaki ifade aşağıdaki derleyici tarafından aşağıdaki koda dönüştürülüyor:

```cpp
co_await prom.yield_value(expr);
```

