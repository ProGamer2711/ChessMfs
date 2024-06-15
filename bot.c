#include "Vector.h"
#include "Constants.h"
#include "Chess.h"
#include <stdio.h>

#define MAX_DEPTH 4

double staticEvaluation(Vector* board, Vector* pieces)
{
    double material=0;
    for(byte i=0;i<pieces->length;i++)
    {
        Piece* piece=pieces->get(pieces,i);
        double tmp_mat=0;

        if(piece->isTaken)continue;

        if(piece->type==KING) tmp_mat=8000;
        if(piece->type==ROOK_1||piece->type==ROOK_2) tmp_mat=5;

        // if the piece is black which wont happen here
        if(!piece->isWhite)tmp_mat*=-1;
        material+=tmp_mat;
    }

    // TBD add evaluation for king freedom
    return material;
}

double min_max(Vector* moves,Vector* board, Vector* pieces, byte depth, byte whitePlayer) // white is maxxer 
{

    if(depth==0)
    {
        // Is game over check needed
        return staticEvaluation(board,pieces);
    }

    if(whitePlayer)
    {
        double maxx=-1e9;
        for(byte i=0;i<pieces->length;i++)
        {
            Piece* currentPiece=pieces->get(pieces,i);

            if(currentPiece->isTaken) continue;
            if(currentPiece->isWhite!=whitePlayer) continue;

            Vector* legalmoves = getLegalMoves(pieces,currentPiece,board);

            for(byte i=0;i<legalmoves->length;i++)
            {
                Coordinate* move=legalmoves->get(legalmoves,i);

                makeMove(moves,board,currentPiece,*move);
                double result=min_max(moves,board,pieces,depth-1,0);

                maxx=max(maxx,result);
                undoMove(moves,board);
            }
        }

        if(maxx==1e9)
        {
            return -depth*1000;
        }
        
        return maxx;
    }
    else
    {
        double minn=1e9;
        for(byte i=0;i<pieces->length;i++)
        {
            Piece* currentPiece=pieces->get(pieces,i);

            if(currentPiece->isTaken) continue;
            if(currentPiece->isWhite!=whitePlayer) continue;

            Vector* legalmoves = getLegalMoves(pieces,currentPiece,board);

            for(byte i=0;i<legalmoves->length;i++)
            {
                Coordinate* move=legalmoves->get(legalmoves,i);
                
                makeMove(moves,board,currentPiece,*move);
                double result=min_max(moves,board,pieces,depth-1,1);

                minn=min(minn,result);
                undoMove(moves,board);
            }
        }

        if(minn==1e9)
        {
            return depth*1000;
        }
        return minn;
    }
}

void blackTurn(Vector* moves,Vector* board, Vector* pieces)
{
    Piece* ansPiece;
    Coordinate ansMove;
    double bestEval=1e9+1;

    printf("Blakc Turn\n");

    for(byte i=0;i<pieces->length;i++)
    {
        Piece* currentPiece=pieces->get(pieces,i);
        if(currentPiece->isTaken) continue;
            if(currentPiece->isWhite!=0) continue;

            Vector* legalmoves = getLegalMoves(pieces,currentPiece,board);

            for(byte i=0;i<legalmoves->length;i++)
            {
                Coordinate* move=legalmoves->get(legalmoves,i);

                printf("Making move:\n");
                makeMove(moves,board,currentPiece,*move);

                printf("Calling minmax:\n");
                double result=min_max(moves,board,pieces,MAX_DEPTH,1);
                printf("%lf ",result);
                if(result<bestEval)
                {
                    ansPiece=currentPiece;
                    ansMove=*move;
                    bestEval=result;
                }

                printf("Undoing move\n");
                undoMove(moves,board);

            }
    }

    printf("Making last move:\n");
    makeMove(moves,board,ansPiece,ansMove);
}