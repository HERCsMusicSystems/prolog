
import studio

program fx #machine := "PrologFX.PrologFXStudio" [fx_start fx_stop file_reader_chooser file_writer_chooser]

#machine fx_start := "fx_start"
#machine fx_stop := "fx_stop"
#machine file_reader_chooser := "file_reader_chooser"
#machine file_writer_chooser := "file_writer_chooser"

end := [[crack [command] [fx_stop]] [fx_start]] .
