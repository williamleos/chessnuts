## Introduction ##

The goal of this program is to provide an intuitive virtual chess table for friendly play across the internet. No rules will be hard-coded, so you can play any variations you please.

Obviously this is not for tournaments. Just like a real chess board, if your opponent goes to the bathroom you can move all his pieces around and run down his clock. Currently I have no plans to simulate the nervous "Oh good, you're back. Shall we continue?" upon your opponent's return.

<a href='http://sites.google.com/site/jacksankey/files/chessnuts_screenshot.jpg?attredirects=0'><img src='http://sites.google.com/site/jacksankey/files/chessnuts_screenshot.jpg?attredirects=0' align='right' width='300px' title='Yes, you can change the default colors.'></img></a>

Chessnuts is fairly basic. It allows you to host a multiplayer session with up to 64 players/observers, and will smoothly update everyone's board as pieces are dragged around. It also includes a chat window and several different setup schemes including traditional, random, and Fischer random (chess960), with even, odd, or uncorrelated symmetry. Since version 0.80, chessnuts now includes a syncronized chess clock (toggle it by pushing the appropriate button or the space bar).

The source is included with each distribution. Feel free to tinker with it as you see fit. It should compile right out of the box with the latest [wxdev-c++](http://wxdsgn.sourceforge.net/).

Special thanks to the makers of xchess, whose pieces I totally swiped (and modified). That's what you get for including a screenshot on sourceforge! Muahahahahaha!



## Controls ##

  * **Left Mouse:** pick up / put down pieces

  * **Right Mouse:** duplicate piece, or delete the piece you're holding (if hovering over an empty square)

  * **Space:** pushes the chess clock's bottom toggle button (but only if you have recently touched the chess board, so it won't mess up chats).