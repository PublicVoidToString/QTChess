# Board representation (bitboard)
<br/>
000 000 000 000 000 000 000 MAX<br/>
000 000 000 000 000 000 000 000<br/>
000 000 000 000 000 000 000 000<br/>
000 000 000 000 000 000 000 000<br/>
000 000 000 000 000 000 000 000<br/>
000 000 000 000 000 000 000 000<br/>
256 512 000 000 000 000 000 000<br/>
001 002 004 008 016 032 064 128

A1 = 2^0 B1 = 2^1 C1 = 2^3 ... H8 = 2^63

# Board representation (Buttons IDs)
<br/>
56 57 58 59 60 61 62 63<br/>
48 49 50 51 52 53 54 55<br/>
40 41 42 43 44 45 46 47<br/>
32 33 34 35 36 37 38 39<br/>
24 25 26 27 28 29 30 31<br/>
16 17 18 19 20 21 22 23<br/>
08 09 10 11 12 13 14 15<br/>
00 01 02 03 04 05 06 07

### In short 2^(button ID) = bitboard

Current board is stored inside chessboard pointer Board* board

//After you go back there is no coming back <br/>

Move representation <br/>
promotion as 4 different moves

New version
100000 000000 0000 00000 000000 0 0000 00 0000000 0 0000000000000000000000  
|||||| |||||| |||| ||||| |||||| | |||| || ||||||| | ||||||||||||||||||||||
moved  moved  QRBN QRBNP PNBRQK E SLSL WB 50rule  3 move counter
from   to     prom capt  moving   WWBB WW   

moved from - old position of moved piece
moved to - new position of moved piece
QRBN - promoted to queen, rook, bishop, knight
QRBNP - capture (queen, rook, bishop, knight)
PNBRQK - a moving piece/attacker (pawn, knight, bishop, rook, queen, king)
E - en passant (as played move)
S/L W/B - short/long castle possible white/black
WW - white won
BW - black won (if WW & WB draw)
50rule - 50 move rule draw (50 moves = 100 plies)
3 - 3 repetition move rule flag
move counter - counts plies (or half-moves)

# Image Sources:
https://www.flaticon.com/free-icon/queen_1626844?related_id=1626879&origin=search <br/>
https://www.flaticon.com/free-icon/chess_11497350?term=chess+pieces&related_id=11497350 <br/>
https://www.flaticon.com/free-icon/rook_1626848?related_id=1626883&origin=search <br/>
https://www.flaticon.com/free-icon/pawn_657487?related_id=657588&origin=search <br/>
https://www.flaticon.com/free-icon/king_3522646?related_id=3522656&origin=search <br/>
https://www.flaticon.com/free-icon/bishop_5077021?term=bishop&page=1&position=1&origin=search&related_id=5077021 </br>

# Evaluation
PeSTO's Evaluation Function based on Pawel Koziol's implementation in TSCP by Tom Kerrigan 
https://rofchade.nl/?p=307
https://www.chessprogramming.org/PeSTO's_Evaluation_Function
https://adamberent.com/chess-board-evaluation/
https://adamberent.com/piece-square-table/

# General algorithms, board representation and ideas
https://www.youtube.com/watch?v=w4FFX_otR-4&t=308s


https://www.codeproject.com/Articles/5313417/Worlds-fastest-Bitboard-Chess-Movegenerator
