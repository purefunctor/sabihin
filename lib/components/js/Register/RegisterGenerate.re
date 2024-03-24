module GenerateSteps = {
  open RegisterGenerateState;

  let to_index = (generateStep: RegisterGenerateState.step_t) =>
    switch (generateStep) {
    | Master => 0
    | Derived => 1
    | Protection => 2
    | Verification => 3
    | EncryptingKeys => 4
    | ExportingKeys => 5
    };

  let step_text = [|
    "Master Key",
    "Derived Key",
    "Protection Key",
    "Verification Key",
    "Encrypting Keys",
    "Exporting Keys",
  |];

  let to_text = (generateStep: RegisterGenerateState.step_t) => {
    generateStep |> to_index |> Belt.Array.getUnsafe(step_text);
  };

  let make_step = (~previous, text) => {
    let className = if (previous) {" color-offwhite"} else {""};
    let icon =
      if (previous) {
        <Icons.CheckFill size="1rem" className="color-success" />;
      } else {
        <Icons.RingResize size="1rem" className="color-primary" />;
      };
    <span
      className={"poppins-regular register-generate-step" ++ className}
      key=text>
      {React.string(text)}
      icon
    </span>;
  };

  let make_finished = text => {
    <span
      className="poppins-regular register-generate-step color-offwhite"
      key=text>
      {React.string(text)}
      <Icons.CheckFill size="1rem" className="color-success" />
    </span>;
  };

  [@react.component]
  let make = (~generateStep: RegisterGenerateState.step_t) => {
    let previousSteps =
      Hooks.useMemoPrevious(
        [||],
        prev_elements => {
          let prev_elements = Belt.Array.copy(prev_elements);

          let index = to_index(generateStep);
          if (index > 0) {
            let prev_index = index - 1;
            let prev_text = Belt.Array.getUnsafe(step_text, prev_index);

            let prev_step = make_step(~previous=true, prev_text);
            Belt.Array.push(prev_elements, prev_step);
          };

          prev_elements;
        },
        [|generateStep|],
      );

    let currentStep =
      React.useMemo1(
        () => {
          let index = to_index(generateStep);
          let text = Belt.Array.getUnsafe(step_text, index);
          make_step(~previous=false, text);
        },
        [|generateStep|],
      );

    <div className="register-generate-steps">
      {React.array(previousSteps)}
      currentStep
    </div>;
  };
};

[@react.component]
let make =
    (
      ~formState: RegisterFormState.t,
      ~generateState: RegisterGenerateState.t,
      ~nextRegisterStep,
    ) => {
  let (generateStep, nextGenerateStep) = RegisterGenerateState.useStep();

  let _ = formState;
  let _ = generateState;

  let _ = nextRegisterStep;
  let _ = nextGenerateStep;

  React.useEffect1(
    () => {
      switch (generateStep) {
      | RegisterGenerateState.ExportingKeys => ()
      | _ => ignore(Js.Global.setTimeout(~f=nextGenerateStep, 500))
      };
      None;
    },
    [|generateStep|],
  );

  <div className="register-generate-content">
    <span className="josefin-sans-title generate-title">
      {React.string("Making Your Keys")}
    </span>
    <GenerateSteps generateStep />
    <p className="poppins-regular register-generate-hint">
      {React.string(
         "Once finished, your encrypted private keys, public keys, and other metadata will be submitted to the server to finalize your registration.",
       )}
    </p>
  </div>;
};
