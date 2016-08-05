
import studio

program fx #machine := "PrologFX.PrologFXStudio" [fx_command fx_start fx_stop file_reader_chooser file_writer_chooser fx_resize_callback]

#machine fx_start := "fx_start"
#machine fx_stop := "fx_stop"
#machine file_reader_chooser := "file_reader_chooser"
#machine file_writer_chooser := "file_writer_chooser"

[[fx_command] [crack [command] [fx_stop]] [fx_start]]

[[fx_resize_callback *x *y] [show "resized: " [*x *y]]]

end := [[fx_command]] .
