
import studio
import fx

program fxg #machine := "fxg.PrologFXGStudio" [
					Viewport MainViewport
					SaveBoard Erase Clean Clean? Repaint
					Rectangle Grid Circle Text Picture Dice Deck
					Location Position Size Scaling Rotation Rounding Side Sides Text Indexing Indexed?
					Select Deselect Selected? Lock Unlock Locked?
					Roll
					ForegroundColour BackgroundColour
					re mv
					r g d
					]


#machine Viewport := "Viewport"
#machine MainViewport := "MainViewport"

#machine Clean := "Clean"
#machine Clean? := "Clean?"
#machine Erase := "Erase"
#machine SaveBoard := "SaveBoard"
#machine Repaint := "Repaint"

#machine Rectangle := "Rectangle"
#machine Grid := "Grid"
#machine Circle := "Circle"
#machine Text := "Text"
#machine Picture := "Picture"
#machine Dice := "Dice"
#machine Deck := "Deck"

[[re : *command] *command [Repaint]]

[[fx_resize_callback *x *y] [Repaint]] ; [show "resized: " [*x *y]] [Repaint]]

end := [[crack [wait 1000] [batch "sonda.txt"]] [fx_command]] .

end := [[crack [wait 1000] [MainViewport mv] [mv Location -100 -100] [Dice r 4] [Grid g] [g Side 1] [g Indexing 1 1 7 7]] [fx_command]] .
