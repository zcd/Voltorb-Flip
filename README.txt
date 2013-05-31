
                    Voltorb Flip

    About
    -----

    Voltorb Flip is an emulation of a mini-game available in the Nintendo
game Pokemon HeartGold/SoulSilver.  It is a card-flip game with 25 cards
where the player flips cards until the points in that round are exhausted
or until the player flips a Voltorb card.  


    Rules
    -----
    
    Flipping a non-Voltorb card will
set your value to that card if your current score is 0 or multiply into
your score otherwise.  Flipping a Voltorb card at any point will result in
a loss of that round.  If there are n > 0 cards have been flipped before a
Voltorb card, then the player restarts at min(n, current level).

    Installation
    ------------

    The most recent executable is included within the 
    'Voltorb Flip/release/' folder.  The source code can be compiled 
through Visual Studio by way of 'Voltorb Flip/Voltorb Flip.sln'.  There is 
also a command-line version under 'Voltorb Flip/Text Version'.

    Licensing
    ---------

    This project was done for the sake of learning C++ and the win32 api
for user interfaces.  I claim no ownership to the images, sprites, or 
dialog text used in the project.
