
import studio

program fx #machine := "PrologFX.PrologFXStudio" [fx_start fx_stop]

#machine fx_start := "fx_start"
#machine fx_stop := "fx_stop"

end := [[crack [command] [fx_stop]] [fx_start]] .
