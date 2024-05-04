open GetStartedPageGenerateHooks;
open GetStartedPageGenerateLogs;

let containerCss = [%cx
  {|
display: flex;
flex-direction: column;
gap: 1rem;
|}
];

module Info = {
  let infoCss = [%cx
    {|
    & > h1 {
      font-size: 2rem;
      margin-top: 0;
      margin-bottom: 0;
    }
    & > p {
      margin-bottom: 0;
    }
    |}
  ];

  [@react.component]
  let make =
    React.memo(() => {
      <article className=infoCss>
        <h1> {React.string("Getting Started")} </h1>
        <p>
          {React.string(
             "To send and receive messages, you must first initialize your cryptography keys.",
           )}
        </p>
        <p>
          {React.string(
             "This is a required one-time process that enables messages to remain private.",
           )}
        </p>
      </article>
    });
};

module Actions = {
  let actionsCss = [%cx
    {|
    display: flex;
    flex-direction: column;
    gap: 1rem;
|}
  ];

  let commonButtonCss = [%cx
    {|
    flex-grow: 1;
    font-family: "Poppins";
    font-size: 1rem;
    line-height: 1.5;
  |}
  ];

  let getStartedButtonCss = Theme.primaryButton ++ " " ++ commonButtonCss;
  let learnMoreButtonCss = Theme.ghostButton ++ " " ++ commonButtonCss;

  [@react.component]
  let make =
    React.memo((~onStart: unit => 'a, ~onRetry: unit => 'a, ~state: State.t) => {
      let onClick = _ => {
        switch (state) {
        | Initial => ignore(onStart())
        | Generating => ()
        | Failed => ignore(onRetry())
        | Finished => ()
        };
      };
      let buttonElement =
        switch (state) {
        | Initial => React.string("Get Started")
        | Generating => <Icons.RingResize size="1.5rem" />
        | Failed => React.string("Retry")
        | Finished => <Icons.CheckLine size="1.5rem" />
        };
      <div className=actionsCss>
        <button onClick className=getStartedButtonCss> buttonElement </button>
        <a href="/learn" className=learnMoreButtonCss>
          {React.string("Learn More")}
        </a>
      </div>;
    });
};

[@react.component]
let make = (~onFinish) => {
  let derivedSecretsStore = DerivedSecretsContext.useContext();
  let (logs, state, generateKeys, clear) = useGenerateKeys();

  let onStart =
    React.useCallback0(_ => {
      open Js.Promise;
      open Promise_syntax;

      let* derivedSecrets = derivedSecretsStore.get();
      switch (derivedSecrets) {
      | Some(derivedSecrets) =>
        generateKeys(derivedSecrets) |> then_(onFinish)
      | None => resolve()
      };
    });

  let onRetry =
    React.useCallback0(_ => {
      clear();
      onStart();
    });

  <div className=containerCss>
    <Info />
    <Actions onStart onRetry state />
    <Logs logs />
  </div>;
};
