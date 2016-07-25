
import studio
import fx

program fxg #machine := "fxg.PrologFXGStudio" [
					Viewport
					SaveBoard Erase Clean Clean?
					Location Position Size Scaling Mode Rotation Side Sides Text
					]


#machine Viewport := "Viewport"
#machine Clean := "Clean"
#machine Clean? := "Clean?"
#machine Erase := "Erase"
#machine SaveBoard := "SaveBoard"

end := [[crack [command] [fx_stop]] [fx_start]] .
