#include "polish_notation.h"

bool  PolishRecordMaster::is_operation(char t)
{
    return (t == '^' ||
            t == '+' ||
            t == '-' ||
            t == '*' ||
            t == '/');
}

int   PolishRecordMaster::get_priority(char op)
{
    if (op == '^' || op == '_')     return 0;
    if (op == '*' || op == '/')     return 1;
    if (op == '+' || op == '-')     return 2;
    if (op == '(' || op == ')')     return 3;
}

bool  PolishRecordMaster::is_right_op(char op)
{
    return (op == '^' || op == '_');
}

bool  PolishRecordMaster::is_uno(char op)
{
    return (op == '_');
}

bool  PolishRecordMaster::is_number(char t)
{
    return (t >= '0' && t <= '9');
}

bool  PolishRecordMaster::more(char op1, char op2)
{
    return (get_priority(op1) < get_priority(op2));
}

bool  PolishRecordMaster::more_or_eq(char op1, char op2)
{
    return (get_priority(op1) <= get_priority(op2));
}

bool  PolishRecordMaster::hasFractionalPart(float val){
    return val - floor(val) > 0 ;
}

float PolishRecordMaster::quick_power(float x, int n)
{
    float ans = 1.0;
    while (n > 0)
    {
        if (n%2==1) ans*=x;
        x*=x;
        n>>=1;
    }
    return ans;
}

float PolishRecordMaster::read_float_from_pos(string &s, int pos, int &nextpos)
{
    bool waspoint = false;
    float r1 = 0, r2 = 0, d = 10;
    for (int i = pos; i < (int)s.length(); ++i)
    {
        if (s[i] == '.' || s[i] == ',')
            waspoint = true;
        else if (is_number(s[i]))
        {
            float c = (float)(s[i] - '0');
            if (!waspoint)
                r1 = r1 * 10 + c;
            else
            {
                r2 += c / d;
                d *= 10;
            }
        }
        else
        {
            nextpos = i;
            return r1 + r2;
        }
    }
}

void  PolishRecordMaster::print_rpn_stack(stack <rpn_stack_elem> ST)
{
    while (!ST.empty())
    {
        rpn_stack_elem el = ST.top();
        if (el.op == RPN_NUM)
            cout << el.x << " ";
        else
            cout << el.op << " ";
        ST.pop();
    }
}

bool  PolishRecordMaster::get_rpn(string req)
{
    req = '(' + req + ')';

    stack <char> st;            // Стек, в который складываются операции
    int len = req.length();
    int i = 0;
    bool canbeuno = true;       // Флаг - может ли быть сл. операция унарной

    for (i = 0; i < len; ++i)
    {
        rpn_stack_elem el;

        if (req[i] == ' ')
            continue;
        else if (req[i] == '(')
        {
            st.push('(');
            canbeuno = true;
        }
        else if (is_number(req[i]))
        {
            int ni;
            el.op = RPN_NUM;
            el.x = read_float_from_pos(req, i, ni);
            RPN_stack.push(el);

            i = ni - 1;
            canbeuno = false;
        }
        else if (is_operation(req[i]))
        {
            char op = req[i];
            if (req[i] == '-' && canbeuno)      // нашли унарный минус
            {
                while (!st.empty())
                {
                    if ( (!is_right_op(st.top()) && more_or_eq(st.top(), '_')) ||
                        (is_right_op(st.top()) && more (st.top(), '_')))
                    {
                        el.op = st.top();
                        el.rpriority = is_right_op(el.op);
                        RPN_stack.push(el);
                        st.pop();
                    }
                    else break;
                }
                st.push('_');
                continue;
            }

            while (!st.empty())
            {
                if ( (!is_right_op(st.top()) && more_or_eq(st.top(), op)) ||
                      (is_right_op(st.top()) && more (st.top(), op)))
                {
                    el.op = st.top();
                    el.rpriority = is_right_op(el.op);
                    RPN_stack.push(el);
                    st.pop();
                }
                else break;
            }
            st.push(op);
            canbeuno = true;
        }
        else if (req[i] == ')')
        {
            while (!st.empty())
            {
                if (st.top() != '(')
                {
                    el.op = st.top();
                    el.rpriority = is_right_op(el.op);
                    RPN_stack.push(el);
                    st.pop();
                }
                else
                {
                    st.pop();
                    break;
                }
            }
            canbeuno = false;
        }
    }

    stack <rpn_stack_elem> outres;
    while (!RPN_stack.empty())
    {
        outres.push(RPN_stack.top());
        RPN_stack.pop();
    }
    RPN_stack = outres;

    return true;
}

float PolishRecordMaster::rpn_count(bool &ok)
{
    stack <float> st;
    ok = true;
    while (!RPN_stack.empty())
    {
        rpn_stack_elem el = RPN_stack.top();

        if (el.op == RPN_NUM)
            st.push(el.x);
        else
        {
            char op = el.op;
            float o2 = st.top(), o1;
            st.pop();
            if (!is_uno(op))
            {
                if (!st.empty())
                {
                    o1 = st.top();
                    st.pop();
                }
                else
                {
                    ok = false;
                    return NAN;
                }
            }

                 if (op == '+') st.push(o1 + o2);
            else if (op == '-') st.push(o1 - o2);
            else if (op == '*') st.push(o1 * o2);
            else if (op == '/')
            {
                if (fabs(o2) > DELTA_ZERO)
                    st.push(o1 / o2);
                else
                {
                    ok = false;
                    return NAN;
                }
            }
            else if (op == '^')
            {
                if (!hasFractionalPart(o2) && o2 > 0)
                    st.push(quick_power(o1, round(o2)));
                else if (o1 >= 0.0)
                    st.push(pow(o1, o2));
                else
                {
                    ok = false;
                    return NAN;
                }
            }
            else if (op == '_')
            {
                st.push(-o2);
            }
            else                // Неопознанные символы
            {
                ok = false;
                return NAN;
            }
        }
        RPN_stack.pop();
    }

    if (!RPN_stack.empty())
        ok = false;

    if (ok)
        return st.top();
    else
        return 0;
}

float PolishRecordMaster::Resolve(string req, bool &ok)
{
    get_rpn(req);

    print_rpn_stack(this->RPN_stack);

    return rpn_count(ok);
}
