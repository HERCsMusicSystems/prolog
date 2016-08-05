
import studio
import fx

program fxg #machine := "fxg.PrologFXGStudio" [
					Viewport MainViewport
					SaveBoard Erase Clean Clean? Repaint
					CreateRectangle Rectangle
					Location Position Size Scaling Rotation Rounding Side Sides Text
					ForegroundColour BackgroundColour
					re mv
					]


#machine Viewport := "Viewport"
#machine MainViewport := "MainViewport"

#machine Clean := "Clean"
#machine Clean? := "Clean?"
#machine Erase := "Erase"
#machine SaveBoard := "SaveBoard"
#machine Repaint := "Repaint"

#machine CreateRectangle := "CreateRectangle"
#machine Rectangle := "CreateRectangle"

[[re : *command] *command [Repaint]]

[[fx_resize_callback *x *y] [show "resized: " [*x *y]] [Repaint]]

end := [[crack [wait 1000] [MainViewport mv]] [fx_command]] .
