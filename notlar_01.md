Normal fonksiyonlar çağrıldığında kodlarının tamamı çalıştırılır. Fonksiyonun çalışmasının sonlanması için
- Kodlarının tamamının çalıştırılması _(void fonksiyonlar)_
- bir _return_ deyiminin yürütülmesi
- fonksiyonun kodundan bir _exception_ gönderilmesi yoluyla çıkılması gerekir.

Oysa _coroutine_'ler kodları birden fazla adımda çalışabilen fonksiyonlardır.
Belirli anlarda, bir coroutine'inin çalışmasını sürdürebilir ya da çalışmasını askıya alabiliriz, yani çalışmasını daha sonra tekrar başlatmak üzere durdurabiliriz. (İngilizcede bu anlamda _suspend_ fiili kullanılıyor, isim hali _suspension_) 
Bir coroutine fonksiyonunun çalışmasını, bir olayı beklemesi gerektiği için, yapılacak başka (daha önemli) işler olduğu için ya da çağıran koda iletmemiz gereken bir ara sonuç olduğu için, daha sonra tekrar başlatmak üzere durdurabiliriz.
