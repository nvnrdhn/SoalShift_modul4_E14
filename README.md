# SoalShift_modul4_E14
# soal1
Terdapat dua fungsi: enkrip dan dekrip yang berfungsi untuk melakukan enkripsi dan dekripsi string dengan metode caesar cipher. Untuk setiap fungsi di fuse, pathfile akan di enkripsi terlebih dahulu kemudian di append dengan path direktori `/home/user/shift4` yang kemudian didapat fullpath file aslinya. Ketika user melakukan command `ls`, nama file asli akan di dekripsi terlebih dahulu lalu kemudian ditampilkan.
# soal2
Pertama, dibuat satu thread untuk membuat file "Videos" dan untuk melakukan join terhadap file video. Untuk join, dilakukan pembacaan file di dalam direktori `/home/user/shift4/` Jika terdapat file yang memiliki ekstensi `001` (yang menandakan awal dari joinnya file video, maka nama dari file video itu akan disimpan disebuah array. Jika terdapat file yang memiliki nama yang sama dengan array tersebut (yang menandakan bagian dari file video yang terpecah) maka dilakukan penggabungan. 

Pada fuse nya sendiri dilakukan perubahan pada readdir, yaitu jika terdapat folder yang bernamakan `Videos` maka akan dihapus menggunakan `rmdir`, sehingga folder `Videos` akan terhapus.

# soal3
Untuk setiap pembacaan direktori, simpan stat dari file asli kedalam suatu struct stat. Kemudian ambil owner dan group ID dari stat tersebut lalu cek apakah group == "rusak" dan owner == "chipset" atau "ic_controller" dan cek apakah file tersebut dapat dibuka dengan melihat flag S_ISUSR pada variabel stat st_mode. Jika ya maka simpan nama file, group, owner, last access time di file `/home/user/shift4/filemiris.txt` kemudian hapus file tersebut.
