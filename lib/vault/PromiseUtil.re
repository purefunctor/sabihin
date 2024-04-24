open Js.Promise;

let ( let* ) = (f, x) => then_(x, f);

let resolve = resolve;
