![Referans bir isim](google.com/img.png)
Caller ve coroutine, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.
Dönüş Nesnesi, coroutine'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. 
Çağıran, coroutine'i devam ettirmek ve coroutine'den yayılan değerleri almak için return nesnesi ile etkileşime girer. 
Geri dönüş nesnesi genellikle tüm çağrılarını coroutine tanıtıcısına devreder.
Coroutine Handle, Coroutine Durumuna ait olmayan bir handle'tır. Coroutine tanıtıcısı aracılığıyla coroutine durumunu devam ettirebilir ve yok edebiliriz.
Korutin durumu, daha önce korutin çerçevesi olarak adlandırdığım şeydir. 
Opak bir nesnedir, yani boyutunu bilmeyiz ve tanıtıcıdan başka bir şekilde erişemeyiz. 
Coroutine state, coroutine'i en son askıya alındığı yerden devam ettirmek için gerekli olan her şeyi saklar. 
Coroutine state ayrıca Promise'i de içerir.
Promise nesnesi, coroutine'in co_await, co_yield ve co_return anahtar sözcükleri aracılığıyla dolaylı olarak iletişim kurduğu şeydir. 
Değerler veya hatalar coroutine'den gönderilirse, önce promise nesnesine ulaşırlar. 
Söz nesnesi, coroutine ile çağıran arasında bir kanal görevi görür, ancak her ikisinin de söze doğrudan erişimi yoktur.
Kuşkusuz, bu ilk bakışta oldukça yoğun görünebilir. Eksiksiz ancak minimal bir örnek, farklı parçaları biraz anlamanıza yardımcı olacaktır
