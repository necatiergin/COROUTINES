## coroutine frame

Bir _coroutine_ başlatıldığında şunlar olur:<br>
_Coroutine_ için gerekli bütün verilerin tutulması için bir `coroutine frame` oluşturulur. Bu bellek alanı genel olarak _heap_'ten edinilir. Ancak derleyiciler, yapabiliyorlar ise `coroutine frame`'i stack üzerinde de oluşturabilirler. Bu optimizasyon tipik olarak _coroutine_'in yaşam  süresinin çağıran kodun yaşam süresinin içinde kaldığı ve derleyicinin `coroutine frame` için ne kadar bellek alanı gerektiğini hesaplayabileceği durumlarda yapılabilir.
_coroutine_'in bütün parametreleri `coroutine frame`'e kopyalanır. Referansların kopyalanması referans semantiği ile olur. Yani değerler kopyalanmaz. _Coroutine_'in çalışması devam ettiği sürece referanslar geçerli durunmda kalmalıdır. Geçersiz _(dangling)_ referanslar oluşabileceğinden referans parametrelerden genel olarak kaçınmak gerekir. 
_promise_ nesnesi _coroutine frame_ içinde oluşturulur. BU nesnenin görevi coroutine'in durum bilgisini tutmak ve coroutine çalışırken kullanılacak özelleştirme _(customization)_
noktaları sağlamaktır.
Bir _promise_ nesnesi yoluyla _coroutine_'in durumu _(state)_ kontrol edilebilir ve takip edilebilir.
<!-- 
-->

