#include <texteditor.h>

void TextEditor::add_symbol(char ch)
{
    text[cursor_str].insert(cursor_ind, &ch, 1);
    ++cursor_ind;

}

void TextEditor::divide_strings()
{
    text.insert(text.begin() + cursor_str + 1, 1,
                text[cursor_str].substr(cursor_ind));
    text[cursor_str] = text[cursor_str].substr(0, cursor_ind);
    ++cursor_str;
    cursor_ind = 0;
}

void TextEditor::delete_symbol()
{
    text[cursor_str].erase(cursor_ind - 1, 1);
    --cursor_ind;
}

void TextEditor::concat_strings()
{
    text[cursor_str - 1] += text[cursor_str];
    text.erase(text.begin() + cursor_str);
    --cursor_str;
}

void TextEditor::do_backspace()
{
    if (cursor_ind == 0)
    {
        if (cursor_str > 0)
        {
            aStack.push(TextAction(STRCONCAT, cursor_str, text[cursor_str-1].length()));
            cursor_ind = text[cursor_str - 1].length();
            concat_strings();
        }
        else {} // nothing to do
    }
    else
    {
        aStack.push(TextAction(DELETED, cursor_str, cursor_ind, text[cursor_str][cursor_ind-1]));
        delete_symbol();
        //text[cursor_str].erase()
    }
}

void TextEditor::undo()
{
    if (aStack.empty())
        return;

    backStack.push(aStack.top());
    TextAction ta = aStack.top();
    aStack.pop();

    if (ta.act == ADDED) // был добавлен символ
    {
        cursor_str = ta.str;
        cursor_ind = ta.index + 1;
        delete_symbol();
    }
    else if (ta.act == NEWLINE)
    {
        cursor_str = ta.str + 1;
        cursor_ind = ta.index;
        concat_strings();
    }
    else if (ta.act == DELETED)
    {
        cursor_str = ta.str;
        cursor_ind = ta.index - 1;
        add_symbol(ta.del_ch);
    }
    else if (ta.act == STRCONCAT)
    {
        cursor_str = ta.str - 1;
        cursor_ind = ta.index;
        divide_strings();
    }

    //print();
}

void TextEditor::redo()
{
    if (backStack.empty())
        return;

    aStack.push(backStack.top());
    TextAction ta = backStack.top();
    backStack.pop();

    cursor_ind = ta.index;
    cursor_str = ta.str;



    if (ta.act == ADDED) // заново добавляем символ
    {

        add_symbol(ta.del_ch);
    }
    else if (ta.act == NEWLINE)
    {
        divide_strings();
    }
    else if (ta.act == DELETED)
    {
        delete_symbol();
    }
    else if (ta.act == STRCONCAT)
    {
        concat_strings();
    }

}

void TextEditor::print()
{
    for (int i = 0; i < text.size(); ++i)
    {
        if (i > 0) cout << endl;
        cout << text[i];
    }
}

void TextEditor::Solve(const char *filename)
{
    int i;

    cursor_str = 0;
    cursor_ind = 0;

    // -------------------------------------------------

    text.push_back("");

    ifstream in(filename);     //открыли для чтения файл


    char ch, nextchar = in.get();
    while (true)
    {
        //;
        if (nextchar != EOF)
            ch = nextchar;
        else
            break;

        nextchar = in.get();
        if (nextchar == EOF)
            break;


        switch (ch)
        {
        case '<':   // left
        {
            if (cursor_ind > 0)
                --cursor_ind;
            else
            {
                if (cursor_str > 0)
                {
                    --cursor_str;
                    cursor_ind = text[cursor_str].length();
                }
                else {} // doing nothing
            }
            break;
        }
        case '>':   // right
        {
            if (cursor_ind < text[cursor_str].length())
                ++cursor_ind;
            else
            {
                if (cursor_str < text.size() - 1)
                {
                    ++cursor_str;
                    cursor_ind = 0;
                }
                else {} // doing nothing
            }
            break;
        }
        case '^':   // up
        {
            if (cursor_str > 0)
            {
                --cursor_str;
                if (cursor_ind > text[cursor_str].length())
                    cursor_ind = text[cursor_str].length();
            }
            break;
        }
        case '|':   // down
        {
            if (cursor_str < text.size() - 1)
            {
                ++cursor_str;
                if (cursor_ind > text[cursor_str].length())
                    cursor_ind = text[cursor_str].length();
            }

            break;
        }
        case '#':   // backspace
        {
            do_backspace();
            break;
        }
        case '@':   // undo
        {
            undo();
            break;
        }
        case '$':   // redo
        {
            redo();
            break;
        }
        case '\n':  // new line
        {
            while (!backStack.empty())
                backStack.pop();

            aStack.push(TextAction(NEWLINE, cursor_str, cursor_ind));

            divide_strings();
            break;
        }

        default:
        {
            while (!backStack.empty())
                backStack.pop();

            aStack.push(TextAction(ADDED, cursor_str, cursor_ind, ch));
            add_symbol(ch);
        }
        } // switch
    }

    print();
    cout << endl;
    //in.close(); //Закрываем файл
}
