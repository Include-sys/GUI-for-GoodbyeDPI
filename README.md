# GoodByeDPI GUI

[GoodByeDPI](https://github.com/ValdikSS/GoodbyeDPI) isimli anti sansür uygulaması için kullanıcı arayüzü ile kolay kullanım sağlar.

> İndirme Bağlantıları
>
>[32 Bit](https://github.com/hex4d0r/GUI-for-GoodbyeDPI/releases/download/v1.0/GoodByeDPI_GUI_32Bit.zip)
>
>[64 Bit](https://github.com/hex4d0r/GUI-for-GoodbyeDPI/releases/download/v1.0/GoodByeDPI_GUI_64Bit.zip)

# Kullanım

Parametrelerin tam olarak kayıt edilebilmesi için Ayarlar penceresini **kapatıp** çalıştırmanız gerekli. Sonraki güncellemede eklenecek. Ana ekrandaki **Log** bölümünden uygulamanın düzgün çalışıp çalışmadığını kontrol edebilirsiniz, herhangi bir problem yaşadığınızda bildirmekten çekinmeyin.

> 64Bit Sürümü kullanmak için VC++ 2017 paketini yüklemeniz gerekmektedir.
> Link: [Visual Studio 2017](https://support.microsoft.com/tr-tr/help/2977003/the-latest-supported-visual-c-downloads)

Başlangıçta özel bir ayar yapmanıza gerek yoktur. **Başlat** butonuna tıklayıp sansürsüz internetin keyfini çıkarabilirsiniz. Özel Ayarlar isteğe bağlı şekilde düzenlenebilir. İlk açılışta en uygun ayar **Ön Tanımlı** olarak ayarlanmıştır.

**Windows DNS adresini kendiniz değiştirmeniz gerekmektedir. Aksi halde kuvvetle muhtemel engellenmiş sitelere erişim sağlayamayacaksınız.**

## Kaynak Koddan Derleme

Yazılım MSVC-2017(64Bit Versiyon) ve MinGW-5.3.0(32Bit Versiyon) versiyonları ile derlenip **test** edilmiştir. Sadece bu derleyicileri kullanma zorunluluğu yoktur. Qt Framework Online Installer ile kurulum gerçekleştirirken, **MSVC ve MinGW derleyicilerinin yüklenmesi için işaretlemeyi unutmayın**.

Qt Creator'ı başlattıktan sonra File - Open File tıklayıp **.pro** uzantılı dosyayı seçin. Sonraki ekranda Configure butonuna basarak derleyiciniz için gerekli ayarlamaları yaptıktan sonra Build butonuna basarak yazılımı derleyebilirsiniz. Ardından Debug veya Release dizininde windeployqt uygulaması ile yazılımın çalışabilmesi için gerekli olan Qt kütüphanelerini otomatik şekilde kopyalayabilirsiniz.

## GoodByeDPI'ı Kaynak Koddan Derleme

> [Google Docs](https://docs.google.com/document/d/1LMGmFVu17NKItqTpJKGKXMhX58xWcCJPezddCo73e7c/edit?usp=sharing)

## İleriki Sürümlerde Eklenmesi Planlanan/Beklenen Özellikler
|Özellik| İyileştirme |
|--| --|
| Çalışma Saatleri Ayarlama |Parametre Sisteminin Yeniden Kodlanması|
| Güncelleme Yöneticisi  |Daha İyi Hata Tespiti|
| |Arayüzde Yapılacak Değişiklikler |

## Bağış

Destek olmak için,
[Patreon](https://www.patreon.com/hex4d0r)