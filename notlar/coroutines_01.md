Normal fonksiyonlar çağrıldığında kodlarının tamamı çalıştırılır. Fonksiyonun çalışmasının sonlanması için
- Kodlarının tamamının çalıştırılması _(void fonksiyonlar)_
- bir _return_ deyiminin yürütülmesi
- fonksiyonun kodundan bir _exception_ gönderilmesi yoluyla çıkılması gerekir.

Oysa _coroutine_'ler kodları birden fazla adımda çalışabilen fonksiyonlardır.
Belirli anlarda, bir _coroutine_'in çalışmasını askıya alabiliriz, yani çalışmasını daha sonra tekrar başlatmak üzere durdurabiliriz. (İngilizcede bu anlamda _suspend_ fiili kullanılıyor, isim hali _suspension_) Sonra çalışmasını tekrar devam ettirebiliriz.
Bir _coroutine_ fonksiyonunun çalışmasını örneğin, 
- bir olayı beklemesi gerektiği için,
- yapılacak başka (daha önemli) işler olduğu için,
- koda iletmemiz gereken bir ara sonuç olduğu için, 
daha sonra tekrar başlatmak üzere durdurabiliriz.

Bu nedenle bir _coroutine_'in başlatılması, bir kısmı tamamlanıncaya kadar çalıştırılmak üzere bir fonksiyonun çalışmasının başlatılması anlamına gelir. 
Çağıran fonksiyon ve _coroutine_, iki yürütme yolu arasında gidip gelerek çalışır. 
Burada iki fonksiyonun kodu paralel olarak çalıştırılmamaktadır. Kontrol akışını ayarlayarak adeta ping-pong oynuyoruz:
- Bir fonksiyon, bir _coroutine_'in mevcut kontrol akışını başlatmaya ya da devam ettirmeye _coroutine_'in deyimlerini başlatarak ya da devam ettirerek karar verebilir.
- Daha sonra bir _coroutine_ çalıştığında, _coroutine_ yürütmesini duraklatmaya veya sonlandırmaya kendisi karar verebilir. Bu da _coroutine_'i başlatan veya devam ettiren fonksiyonun kontrol akışına devam ettiği anlamına gelir.

- Bir _coroutine_'in en basit şeklinde, hem ana kontrol akışı hem de _coroutine_'in kontrol akışı aynı _thread_'de çalışır. 
- Birden fazla _thread_ kullanmak zorunda değiliz ve eşzamanlı erişimle _(synchronosied access)_ uğraşmak zorunda değiliz. Ancak, _coroutine_'leri farklı _thread_'lerde çalıştırmak mümkündür. 
- Hatta bir _coroutine_'i daha önce askıya alındığı yerden farklı bir _thread_'de devam ettirebiliriz.
coroutine'ler ortogonal bir özelliktir, ancak birden fazla _thread_ birlikte kullanılabilir.
-bir _coroutine_ kullanmak, arka planda zaman zaman başlattığınız ve devam ettirdiğimiz bir fonksiyona sahip olmak gibidir. 
Bununla birlikte, bir _coroutine_'in ömrü iç içe geçmiş kapsamların ötesine geçtiğinden, bir _coroutine_ aynı zamanda durumunu bir bellekte saklayan ve bununla başa çıkmak için bir API sağlayan bir nesnedir.

- Bir fonksiyonun _coroutine_ olarak ele alınabilmesi için tanımmında aşağıdaki anahtar sözcüklerden en az birine sahip olması gerekir.
- co_await
- co_yield
- co_return

Bir _coroutine_ içinde bu anahtar sözcüklerden hiçbirinin gerekli olmaması durumunda, sonunda açıkça bir 
```
co_return; 
```
deyimi yazmamız gerekir.

_Coroutine_'ler bir _stack_ yapısında değiller _(stackless coroutine)_. Bir _coroutine_'i bir _coroutine_ içinden çalıştırıp onu çalıştıran _coroutine_'i _suspend_ etmeden, _suspend_ edemeyiz.<br>
_coroutine_ fonksiyonlar için aşağıdaki kısıtlamalar söz konusudur:
- _main_ fonksiyonu bir _coroutine_ olamaz.
- _variadic_ bir fonksiyon bir _coroutine_ olamaz.
- Sınıfların _constructor_'ları _coroutine_ olamaz.
- Bir sınıfın _destructor_'ı coroutine olamaz.
- _coroutine_ bir _constexpr_ fonksiyon olamaz.
- _coroutine_ bir _consteval_ fonksiyon olamaz.
- _coroutine_'ler _auto return type_  kullanamazlar.
- _coroutine_'ler normal _return_ deyimlerini kullanamazlar.

- [Bir _coroutine_ genellikle çağıran için _coroutine_ arayüzü _(coroutine interface)_ olarak hizmet veren bir nesne döndürür.](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_interface.md)
_coroutine_'in amacına ve kullanımına bağlı olarak, bu nesne zaman zaman bağlamı askıya alan veya değiştiren çalışan bir görevi, zaman zaman değer üreten bir üreteci veya tembel olarak _(lazily)_ ve talep üzerine bir veya daha fazla değer döndüren bir fabrikayı temsil edebilir.<br>



devam edecek...


