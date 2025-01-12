0000  COPY      START     0         
0000  FIRST     STL       RETADR    17202b	
0003            LDB       #LENGTH    69202b	
0006            BASE      LENGTH    
0006  CLOOP     +JSUB     RDREC     4B100035	
000A            LDA       LENGTH    032024	
000D            COMP      #0        290000	
0010            JEQ       ENDFIL    332007	
0013            +JSUB     WRREC     4B10005C	
0017            J         CLOOP     3f2fec	
001A  ENDFIL    LDA       EOF       032010	
001D            STA       BUFFER    0f2014	
0020            LDA       #3        010000	
0023            STA       LENGTH    0f200b	
0026            +JSUB     WRREC     4B10005C	
002A            J         @RETADR    3e2001	
002D  EOF       BYTE      C'EOF'    
002E  RETADR    RESW      1         
0031  LENGTH    RESW      1         
0034  BUFFER    RESB      4096      
0035  RDREC     CLEAR     X         b410	
0037            CLEAR     A         b400	
0039            CLEAR     S         b440	
003B            +LDT      #4096     75100000	
003F  RLOOP     TD        INPUT     e32019	
0042            JEQ       RLOOP     332ffa	
0045            RD        INPUT     db2013	
0048            COMPR     A,S       a004	
004A            JEQ       EXIT      332008	
004D            STCH      BUFFER,X    5bafe4	
0050            TIXR      T         b850	
0052            JLT       RLOOP     3b2fea	
0055  EXIT      STX       LENGTH    132fd9	
0058            RSUB                4c0000	
005B  INPUT     BYTE      X'F1'     
005C  WRREC     CLEAR     X         b410	
005E            LDT       LENGTH    772fd0	
0061  WLOOP     TD        OUTPUT    e32011	
0064            JEQ       WLOOP     332ffa	
0067            LDCH      BUFFER,X    53afca	
006A            WD        OUTPUT    df2008	
006D            TIXR      T         b850	
006F            JLT       WLOOP     3b2fef	
0072            RSUB                4c0000	
0075  OUTPUT    BYTE      X'05'     
0076            END       FIRST     
