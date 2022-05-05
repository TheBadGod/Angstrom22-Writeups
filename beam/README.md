# BEAM

Just disassemble the elixir code:

```
-file("lib/angstrom.ex", 1).

-module('Elixir.Angstrom.CLI').

-compile([no_auto_import]).

-export(['__info__'/1, check/0, main/0, main/1]).

-spec '__info__'(attributes |
                 compile |
                 functions |
                 macros |
                 md5 |
                 exports_md5 |
                 module |
                 deprecated) -> any().

'__info__'(module) -> 'Elixir.Angstrom.CLI';
'__info__'(functions) ->
    [{check, 0}, {main, 0}, {main, 1}];
'__info__'(macros) -> [];
'__info__'(exports_md5) ->
    <<"lc\203Aò:copyright:a5e\233\213\002\225\216ú\002">>;
'__info__'(Key = attributes) ->
    erlang:get_module_info('Elixir.Angstrom.CLI', Key);
'__info__'(Key = compile) ->
    erlang:get_module_info('Elixir.Angstrom.CLI', Key);
'__info__'(Key = md5) ->
    erlang:get_module_info('Elixir.Angstrom.CLI', Key);
'__info__'(deprecated) -> [].

check() ->
    _input@1 =
        'Elixir.Enum':map('Elixir.String':to_charlist(get_input()),
                          fun (_x@1) -> _x@1 + 1 end),
    case _input@1 of
        [103, 106, 115, 102, 120, 112, 115, 108, 116] ->
            get_flag();
        _ -> 'Elixir.IO':puts(<<"Sorry, no flag for you">>)
    end.

get_flag() ->
    'Elixir.IO':puts('Elixir.File':'read!'(<<"flag.txt">>)).

get_input() ->
    'Elixir.String':trim('Elixir.IO':gets(<<"Password: ">>)).

main() -> main([]).

main(_args@1) -> check().
```

So it checks that our input + 1 is equal to `[103, 106, 115, 102, 120, 112, 115,
108, 116]`, which means we just subtract one from each of those numbers and make
them into bytes to get the password `fireworks`.

This gave us the flag `actf{elixir_is_awesome}`
