
import studio

program fx #machine := "PrologFX.PrologFXStudio" [fx_command fx_start fx_stop file_reader_chooser file_writer_chooser fx_resize_callback android_storage acd]

#machine fx_start := "fx_start"
#machine fx_stop := "fx_stop"
#machine file_reader_chooser := "file_reader_chooser"
#machine file_writer_chooser := "file_writer_chooser"
#machine android_storage := "android_storage"
#machine acd := "acd"

[[fx_command] [crack [command] [fx_stop]] [fx_start]]

end := [[fx_command]] .
