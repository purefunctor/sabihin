module External = {
  [@mel.send.pipe: array('a)] external arrayPush: 'a => unit = "push";
  [@mel.send.pipe: array('a)] [@mel.return nullable]
  external arrayPop: option('a) = "pop";
};

type action('a) =
  | Push(int => 'a)
  | Pop(option((int, 'a)) => unit)
  | Replace((int, 'a) => 'a)
  | Clear;

type t('a) = {
  stack: array('a),
  length: int,
  push: (int => 'a) => unit,
  pop: (option((int, 'a)) => unit) => unit,
  replace: ((int, 'a) => 'a) => unit,
  clear: unit => unit,
};

let useStack = (): t('a) => {
  let (stack, act) =
    React.useReducer(
      (oldStack, action: action(_)) =>
        switch (action) {
        | Push(f) =>
          let newStack = Array.copy(oldStack);
          let length = Array.length(newStack);
          let v = f(length);
          newStack |> External.arrayPush(v);
          newStack;
        | Pop(k) =>
          let newStack = Array.copy(oldStack);
          let index = Js.Math.max_int(0, Array.length(newStack) - 1);
          k(
            newStack |> External.arrayPop |> Option.map(item => (index, item)),
          );
          newStack;
        | Replace(f) =>
          let newStack = Array.copy(oldStack);
          let index = Js.Math.max_int(0, Array.length(newStack) - 1);
          let f = f(index);
          let lastItem = newStack |> External.arrayPop;
          switch (lastItem) {
          | Some(lastItem) => newStack |> External.arrayPush(f(lastItem))
          | None => ()
          };
          newStack;
        | Clear => [||]
        },
      [||],
    );

  let length = React.useMemo1(() => Array.length(stack), [|stack|]);
  let push = v => act(Push(v));
  let pop = k => act(Pop(k));
  let replace = f => act(Replace(f));
  let clear = () => act(Clear);

  {stack, length, push, pop, replace, clear};
};
