open Store;
open Types_universal;

include MakeContext({
  type t = Definitions_t.session_kind;

  let use = () => {
    let get = _ => `Guest;
    let set = _ => ();
    let subscribe = (_, _) => ();
    {get, set, subscribe};
  };
});
