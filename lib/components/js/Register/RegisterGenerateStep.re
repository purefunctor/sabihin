open RegisterGenerateState;

let keysToIndex = (keys: keys_t) => {
  switch (keys) {
  | Master => 0
  | Derived => 1
  | Protection => 2
  | Verification => 3
  | EncryptingKeys => 4
  | ExportingKeys => 5
  };
};

let keysText = [|
  "Master Key",
  "Derived Key",
  "Protection Key",
  "Verification Key",
  "Encrypting Keys",
  "Exporting Keys",
|];

let makeStep = (~finished=false, text) => {
  let className = if (finished) {" color-offwhite"} else {""};
  let icon =
    if (finished) {
      <Icons.CheckFill size="1rem" className="color-success" />;
    } else {
      <Icons.RingResize size="1rem" className="color-primary" />;
    };

  <FramerMotion.span
    initial={"y": 100, "opacity": 0.0}
    animate={"y": 0, "opacity": 1.0}
    transition={"ease": "easeOut"}
    className={"poppins-regular register-generate-step" ++ className}
    key=text>
    {React.string(text)}
    icon
  </FramerMotion.span>;
};

let useKeys = generateStep => {
  // Note: This is a pure hook that makes use of internal mutability.
  //
  // In summary, it performs the following actions:
  // * Initializes an array of React elements of length n;
  // * For each generateStep change, it sets the state of the
  //   corresponding current (and previous) step in the array
  //   using setUnsafe.
  //
  // This approach ensures that the array need not be constructed
  // from scratch every time generateStep changes. Consequently,
  // this minimizes the number of times the makeStep function is
  // called.
  //
  // Note: The array is copied on each state change to trigger a
  // proper component re-render instead of solely relying on each
  // generateStep change.
  Hooks.useMemoPrevious(
    () => Belt.(Array.make(Array.length(keysText), React.null)),
    keysElements => {
      let keysElements = Belt.Array.copy(keysElements);

      switch (generateStep) {
      | Keys(keys) =>
        let index = keysToIndex(keys);
        let text = Belt.Array.getUnsafe(keysText, index);
        let step = makeStep(~finished=false, text);
        Belt.Array.setUnsafe(keysElements, index, step);

        if (index > 0) {
          let prev_index = keysToIndex(keys) - 1;
          let prev_text = Belt.Array.getUnsafe(keysText, prev_index);
          let prev_step = makeStep(~finished=true, prev_text);
          Belt.Array.setUnsafe(keysElements, prev_index, prev_step);
        };
      | Finished =>
        let prev_index = Belt.Array.length(keysText) - 1;
        let prev_text = Belt.Array.getUnsafe(keysText, prev_index);
        let prev_step = makeStep(~finished=true, prev_text);
        Belt.Array.setUnsafe(keysElements, prev_index, prev_step);
      };

      keysElements;
    },
    [|generateStep|],
  );
};

let useFinal = generateStep => {
  React.useMemo1(
    () => {
      switch (generateStep) {
      | Finished =>
        <FramerMotion.span
          initial={"y": 100, "opacity": 0.0}
          animate={"y": 0, "opacity": 1.0}
          transition={"ease": "easeOut"}
          className="poppins-regular">
          {React.string("Done!")}
        </FramerMotion.span>
      | _ => React.null
      }
    },
    [|generateStep|],
  );
};

[@react.component]
let make = (~generateStep) => {
  let keys = useKeys(generateStep);
  let final = useFinal(generateStep);
  <div className="register-generate-steps"> {React.array(keys)} final </div>;
};
