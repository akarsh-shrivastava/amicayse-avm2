#include "transformer.h"
#include <iostream>
Transformer::Transformer(ParseTreeNode* q){
    this->root = q;
}

std::vector<ParseTreeNode*> Transformer::transformS(ParseTreeNode* p){
    std::vector<ParseTreeNode*> ret;
    std::vector<TreeNode*> children = p->children;
    for(std::vector<TreeNode*>::iterator i=children.begin(); i!=children.end(); ++i){
        ParseTreeNode *t = (ParseTreeNode*)(*i);
        if(t->nonTerminal == inst){
            ParseTreeNode *i = transformInst(t);
            if (i)
                ret.push_back(i);
        }
        else if(t->nonTerminal == s){
            std::vector<ParseTreeNode*> temp = transformS(t);
            ret.insert(ret.end(), temp.begin(), temp.end());
        }
    }

    delete p;
    return ret;
}

ParseTreeNode* Transformer::transformInst(ParseTreeNode* p){
    ParseTreeNode *first_child = (ParseTreeNode*)*(p->children.begin());
    if(first_child->terminal->type == DELIMITOR){
        delete p;
        return NULL;
    }
    else if(first_child->terminal->type == IDENTIFIER){
        for(std::vector<TreeNode*>::iterator itr=p->children.begin()+1; itr!=p->children.end()-1; ++itr){
            first_child->add_child(*itr);
        }
        delete p;
        ParseTreeNode *first_child_tran = transformInst_(first_child);
        return first_child_tran;
    }
    return p;
}

ParseTreeNode* Transformer::transformInst_(ParseTreeNode* p){
    if(p->children.empty()){
        return p;
    }
    ParseTreeNode *first_child = (ParseTreeNode*)*(p->children.begin());
    ParseTreeNode *first_grand_child = (ParseTreeNode*)*(first_child->children.begin());

    if (first_grand_child->ter){
        if(first_grand_child->terminal->type == COLON){
            first_grand_child->add_child(p);
            p->children.clear();
            delete first_child;
            return first_grand_child;
        }
    }
    else{
        ParseTreeNode *op_list_pt = (ParseTreeNode*)*(first_grand_child->children.begin());
        delete first_child;
        p->children.clear();
        if(first_grand_child){
            std::vector<ParseTreeNode*> ret_ops = transformOps(first_grand_child);
            p->children.insert(p->children.end(), ret_ops.begin(), ret_ops.end());
        }
    }
    return p;
}

std::vector<ParseTreeNode*> Transformer::transformOps(ParseTreeNode* p){
    std::vector<ParseTreeNode*> ret_ops, ret;
    if(p->nonTerminal == ops){
        ParseTreeNode *op1, *more_ops=NULL;
        op1 = (ParseTreeNode*)*(p->children.begin());
        if(p->children.size()>1){
            more_ops = (ParseTreeNode*)*(p->children.begin()+1);
        }
        p->children.clear();
        p->add_child(transformOp(op1));
        if(more_ops){
            ret = transformOps_(more_ops);
            p->children.insert(p->children.end(), ret.begin(), ret.end());
            //p->add_child(more_ops);
        }
        ret_ops.insert(ret_ops.end(), p->children.begin(), p->children.end());
        delete p;
    }
    return ret_ops;
}

std::vector<ParseTreeNode*> Transformer::transformOps_(ParseTreeNode* p){
    std::vector<ParseTreeNode*> tran_ops;
    p->children.erase(p->children.begin());
    tran_ops.push_back(transformOp((ParseTreeNode*)*(p->children.begin())));
    if(p->children.size()>1){
        ParseTreeNode *more_ops = (ParseTreeNode*)*(p->children.begin()+1);
        std::vector<ParseTreeNode*> ret = transformOps_(more_ops);
        tran_ops.insert(tran_ops.end(), ret.begin(), ret.end());
    }
    return tran_ops;
}

ParseTreeNode* Transformer::transformOp(ParseTreeNode* p){
    int size = p->children.size();
    ParseTreeNode *ret = (ParseTreeNode*)*(p->children.begin());
    switch(size){
        case 1: delete p; return ret;
        case 2: ret->add_child(transformNumconst((ParseTreeNode*)*(p->children.begin()+1))); delete p; return ret;
        case 3: ret->add_child(transformNumconst((ParseTreeNode*)*(p->children.begin()+1)));
                p->children.erase(p->children.begin()+2); delete p; return ret;
    }
    return p;
}

ParseTreeNode* Transformer::transformNumconst(ParseTreeNode* p){
    ParseTreeNode *ret = (ParseTreeNode*)*(p->children.begin());
    delete p;
    return ret;
}

ParseTreeNode* Transformer::get_ast(){
    ParseTreeNode *p = new ParseTreeNode(s);
    std::vector<ParseTreeNode*> temp = transformS(root);
    p->children.insert(p->children.end(), temp.begin(), temp.end());

    return p;
}