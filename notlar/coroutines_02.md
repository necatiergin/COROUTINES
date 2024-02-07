![relations](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_realations.png)

Yukarıdaki resmi açıklayalım: <br>

`Caller` ve `Coroutine`, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.<br>
`Return Object`, `Coroutine`'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. <br>
`Caller`, `Coroutine`'i devam ettirmek ve `Coroutine`'den gönderilen değerleri almak için `Return Object` ile etkileşime girer.<br> 
`Return Object` genellikle tüm çağrılarını `coroutine handle`'a yönlendirir.<br>
`Coroutine Handle`, `Coroutine State`'e ait olmayan bir _handle_'dır.<br> 
`Coroutine Handle` aracılığıyla `Coroutine State`i devam ettirebilir ve yok edebiliriz. <br>
`Coroutine State`, _coroutine frame_ olarak adlandırılan varlıktır. Opak bir nesnedir, yani boyutunu bilemeyiz ve handle'ı kullanmadan başka bir şekilde erişemeyiz. `Coroutine State`'e  erişmenin tek yolu _Coroutine Handle_'dır.<br>
`Coroutine State`, `Coroutine`'i en son askıya alındığı _(durdurulduğu- suspend edildiği)_ yerden devam ettirmek için gerekli olan tüm bilgileri saklar. <br>
`Coroutine State` ayrıca _Promise_ nesnesini de içerir.
`Promise` nesnesi, `Coroutine`'in _co_await_, _co_yield_ ve _co_return_ ifadeleri aracılığıyla dolaylı olarak iletişim kurduğu varlıktır. <br>
_Değerler_ veya _exception_'lar `Coroutine`'den gönderildiğinde önce `Promise` nesnesine ulaşırlar. <br> 
`Promise` nesnesi, `Coroutine` ile `Caller` arasında bir kanal görevi görür, ancak her ikisinin de `Promise` nesnesine doğrudan erişimi yoktur.<br>
