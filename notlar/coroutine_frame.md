## coroutine frame

Bir _coroutine_ başlatıldığında şunlar olur:<br>
_Coroutine_ için gerekli bütün verilerin tutulması için bir `coroutine frame` oluşturulur. Bu bellek alanı genel olarak _heap_'ten edinilir. Ancak derleyiciler, yapabiliyorlar ise `coroutine frame`'i stack üzerinde de oluşturabilirler. Bu optimizasyon tipik olarak _coroutine_'in yaşam  süresinin çağıran kodun yaşam süresinin içinde kaldığı ve derleyicinin `coroutine frame` için ne kadar bellek alanı gerektiğini hesaplayabileceği durumlarda yapılabilir.

<!-- All parameters of the coroutine are copied into the frame.
Note that references are copied as references; it is not that their values are copied. This means that the
arguments that the reference parameters refer to have to be valid as long as the coroutine is running.
The advice is to never declare coroutine parameters as references. Otherwise, fatal runtime errors with
undefined behavior may occur.
• The promise object is created inside the frame. Its purpose is to store the state the coroutine and provide
hooks for customization while the coroutine is running.
You can think of these objects as a “coroutine state controller” (an object that controls the behavior of
the coroutine and can be used to track its state).2
Figure 15.1 visualizes this initialization and shows which customization points of the promise object are
used while the coroutine is running -->

