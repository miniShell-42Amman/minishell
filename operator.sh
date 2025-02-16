Here are all possible series of operations using the operators <, <<, >, >>, and |:

Two Operators:
< < INVALID bash: syntax error near unexpected token `<'
< << INVALID bash: syntax error near unexpected token `<<'
< > INVALID bash: syntax error near unexpected token `>'
< >> INVALID bash: syntax error near unexpected token `>>'
< | INVALID bash: syntax error near unexpected token `|'
<< < INVALID bash: syntax error near unexpected token `<'
<< << INVALID bash: syntax error near unexpected token `<<'
<< > INVALID bash: syntax error near unexpected token `>'
<< >> INVALID bash: syntax error near unexpected token `>>'
<< | INVALID bash: syntax error near unexpected token `|'
> < INVALID bash: syntax error near unexpected token `<'
> << INVALID bash: syntax error near unexpected token `<<'
> > INVALID  bash: syntax error near unexpected token `>'
> >> INVALID bash: syntax error near unexpected token `>>'
> | INVALID bash: syntax error near unexpected token `|'
>> < INVALID bash: syntax error near unexpected token `<'
>> << INVALID bash: syntax error near unexpected token `<<'
>> > INVALID bash: syntax error near unexpected token `>'
>> >> INVALID bash: syntax error near unexpected token `>>'
>> | INVALID bash: syntax error near unexpected token `|'
| < VALID
| << VALID 
| > VALID
| >> VALID
| | INVALID bash: syntax error near unexpected token `|'

Three Operators:

< < <
< < <<
< < >
< < >>
< < |
< << <
< << <<
< << >
< << >>
< << |
< > <
< > <<
< > >
< > >>
< > |
< >> <
< >> <<
< >> >
< >> >>
< >> |
< | <
< | <<
< | >
< | >>
< | |
<< < <
<< < <<
<< < >
<< < >>
<< < |
<< << <
<< << <<
<< << >
<< << >>
<< << |
<< > <
<< > <<
<< > >
<< > >>
<< > |
<< >> <
<< >> <<
<< >> >
<< >> >>
<< >> |
<< | <
<< | <<
<< | >
<< | >>
<< | |
> < <
> < <<
> < >
> < >>
> < |
> << <
> << <<
> << >
> << >>
> << |
> > <
> > <<
> > >
> > >>
> > |
> >> <
> >> <<
> >> >
> >> >>
> >> |
> | <
> | <<
> | >
> | >>
> | |
>> < <
>> < <<
>> < >
>> < >>
>> < |
>> << <
>> << <<
>> << >
>> << >>
>> << |
>> > <
>> > <<
>> > >
>> > >>
>> > |
>> >> <
>> >> <<
>> >> >
>> >> >>
>> >> |
>> | <
>> | <<
>> | >
>> | >>
>> | |
| < <
| < <<
| < >
| < >>
| < |
| << <
| << <<
| << >
| << >>
| << |
| > <
| > <<
| > >
| > >>
| > |
| >> <
| >> <<
| >> >
| >> >>
| >> |
| | <
| | <<
| | >
| | >>
| | |

edge cases:
bash->    echo "'$HOME$PWD'"  --->   '/root/root'
mini->    echo "'$HOME$PWD'"  --->   signal SIGSEGV (Address boundary error)

bash->    echo "'$HOME $PWD'"  --->   '/root /root'
mini->    echo "'$HOME $PWD'"  --->   signal SIGSEGV (Address boundary error)