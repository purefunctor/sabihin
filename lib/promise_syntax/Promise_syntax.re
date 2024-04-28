let resolve = Js.Promise.resolve;

let ( let* ) = (x, f) => x |> Js.Promise.then_(f);

let (let+) = (x, f) => x |> Js.Promise.then_(x => resolve(f(x)));
