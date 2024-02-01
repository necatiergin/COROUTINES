Normal fonksiyonlar çağrıldığında kodlarının tamamı çalıştırılır. Fonksiyonun çalışmasının sonlanması için
- Kodlarının tamamının çalıştırılması _(void fonksiyonlar)_
- bir _return_ deyiminin yürütülmesi
- fonksiyonun kodundan bir _exception_ gönderilmesi yoluyla çıkılması gerekir.

Oysa _coroutine_'ler kodları birden fazla adımda çalışabilen fonksiyonlardır.
Belirli anlarda, bir _coroutine_'inin çalışmasını sürdürebilir ya da çalışmasını askıya alabiliriz, yani çalışmasını daha sonra tekrar başlatmak üzere durdurabiliriz. (İngilizcede bu anlamda _suspend_ fiili kullanılıyor, isim hali _suspension_) 
Bir _coroutine_ fonksiyonunun çalışmasını, bir olayı beklemesi gerektiği için, yapılacak başka (daha önemli) işler olduğu için ya da çağıran koda iletmemiz gereken bir ara sonuç olduğu için, daha sonra tekrar başlatmak üzere durdurabiliriz.

Bu nedenle bir _coroutine_'in başlatılması, bir kısmı tamamlanıncaya kadar çalıştırılmak üzere bir fonksiyonun çalışmasının başlatılması anlamına gelir. 
Çağıran fonksiyon ve _coroutine_, iki yürütme yolu arasında gidip gelerek çalışır. 
Burada iki fonksiyonun kodu paralel olarak çalıştırılmamaktadır. Kontrol akışını ayarlayarak adeta  ping-pong oynuyoruz:
- Bir fonksiyon, bir _coroutine_'in mevcut kontrol akışını başlatmaya ya da devam ettirmeye _coroutine_'in deyimlerini başlatarak ya da devam ettirerek karar verebilir.
- Daha sonra bir _coroutine_ çalıştığında, _coroutine_ yürütmesini duraklatmaya veya sonlandırmaya kendisi karar verebilir. Bu da _coroutine_'i başlatan veya devam ettiren fonksiyonun kontrol akışına devam ettiği anlamına gelir.

Bir coroutine'in en basit şeklinde, hem ana kontrol akışı hem de coroutine'in kontrol akışı aynı iş parçacığında çalışır. 
Çoklu iş parçacığı kullanmak zorunda değiliz ve eşzamanlı erişimle uğraşmak zorunda değiliz. 
Ancak, korutinleri farklı iş parçacıklarında çalıştırmak mümkündür. 
Hatta bir coroutine'i daha önce askıya alındığı yerden farklı bir iş parçacığında devam ettirebilirsiniz. 
Korutinler ortogonal bir özelliktir, ancak birden fazla iş parçacığı ile birlikte kullanılabilir.
Etkili bir şekilde, bir coroutine kullanmak, arka planda zaman zaman başlattığınız ve devam ettirdiğiniz bir fonksiyona sahip olmak gibidir. 
Bununla birlikte, bir coroutine'in ömrü iç içe geçmiş kapsamların ötesine geçtiğinden, bir coroutine aynı zamanda durumunu bir bellekte saklayan ve bununla başa çıkmak için bir API sağlayan bir nesnedir.

- Bir fonksiyonun _coroutine_ olarak ele alınabilmesi için tanımmında aşağıdaki anahtar sözcüklerden en az birine sahip olması gerekir.
- co_await
- co_yield
- co_return

Bir _coroutine_ içinde bu anahtar sözcüklerden hiçbirinin gerekli olmaması durumunda, sonunda açıkça bir 
```
co_return; 
```
deyimi yazmamız gerekir.

- Bir _coroutine_ genellikle çağıran için _coroutine_ arayüzü _(coroutine interface)_ olarak hizmet veren bir nesne döndürür. 
Coroutine'in amacına ve kullanımına bağlı olarak, bu nesne zaman zaman bağlamı askıya alan veya değiştiren çalışan bir görevi, 
zaman zaman değer üreten bir üreteci veya tembel olarak ve talep üzerine bir veya daha fazla değer döndüren bir fabrikayı temsil edebilir.

Hala max parametresine kadar olan değerler üzerinde döngü yapan bir tür fonksiyonumuz var. 
Ancak, iki şey sıradan fonksiyonlardan farklıdır:
- Döngünün içinde print ifadesinden sonra, coroutine'i askıya alan ve devam ettirilene kadar bloke eden bir co_await ifadesi vardır. 
Buna askıya alma noktası denir.
Askıya alma çağrısının tam davranışı co_await'ten hemen sonraki ifade tarafından tanımlanır. 
Bu, programcıların askıya alma işleminin tam davranışını kontrol etmesini sağlar.
Şimdilik, askıya almayı kabul eden ve kontrolü çağırana geri veren std::suspend_always türünde varsayılan olarak oluşturulmuş bir nesne kullanacağız. 
Ancak, co_await'e özel operandlar ileterek askıya almayı reddedebilir veya bunun yerine başka bir coroutine'i devam ettirebilirsiniz.

- Coroutine'in geri dönüş ifadesi olmamasına rağmen, CoroTask adında bir geri dönüş tipi vardır. 
Bu tür, coroutine'i çağıran için coroutine arayüzü olarak hizmet eder. 
Dönüş türünü auto olarak bildiremeyeceğimize dikkat edin.
Geri dönüş türü gereklidir, çünkü çağıranın coroutine ile başa çıkabilmesi için bir arayüze ihtiyacı vardır
(devam ettirmek gibi). C++20'de, coroutine arayüz tipleri programcı tarafından sağlanmalıdır (veya bir
üçüncü taraf kütüphanesi). Bunun nasıl uygulandığını daha sonra göreceğiz. Plan, yaklaşan C++ standartlarının kütüphanelerinde bazı standart coroutine arayüz türleri sağlayacaktır.


