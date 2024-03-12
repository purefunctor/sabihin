module Kind = {
  type kind =
    | Register
    | Login;
};

[@react.component]
let make = (~kind: Kind.kind, ~children) => {
  let action =
    switch (kind) {
    | Register => "/api/register"
    | Login => "/api/login"
    };
  <form action method_="post"> children </form>;
};

include Kind;
