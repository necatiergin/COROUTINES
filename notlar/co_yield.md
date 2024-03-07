_co_yield_ operatörünü kullanarak bir _coroutine_'in kendisini çağıran koda değer ya da değerler döndürmesini sağlayabiliriz:

_co_yield_ `C++20` standardı ile dile eklenen bir anahtar sözcük.

```cpp
co_yield expr;
```

biçiminde kullanılıyor. Yukarıdaki ifade aşağıdaki derleyici tarafından aşağıdaki koda dönüştürülüyor:

```cpp
co_await prom.yield_value(expr);
```

Böylece _promise_ nesnesinin _co_yield_ operatörünün operandı olan ifadeyi kullanmasını sağlamış oluyoruz.
