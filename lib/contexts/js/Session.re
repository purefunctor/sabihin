let extractStore = store => {
  switch (store) {
  | Some(store) => store
  | None => failwith("Missing SessionStore.Provider!")
  };
};

let useSession = () => {
  let store = React.useContext(SessionStore.context) |> extractStore;

  let (session, setSession) = React.useState(() => store.get());

  React.useEffect1(
    () => {Some(store.subscribe(() => setSession(_ => store.get())))},
    [||],
  );

  (session, store.set);
};
