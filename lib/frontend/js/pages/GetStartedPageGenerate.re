open GetStartedPageGenerateHooks;
open Promise_syntax;

let articleCss = [%cx
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

let buttonGroupCss = [%cx
  {|
display: flex;
flex-direction: column;
gap: 1rem;
|}
];

let getStartedButtonCss =
  Theme.primaryButton
  ++ " "
  ++ [%cx
    {|
flex-grow: 1;
font-family: "Poppins";
font-size: 1rem;
line-height: 1.5;
|}
  ];

let learnMoreButtonCss =
  Theme.ghostButton
  ++ " "
  ++ [%cx
    {|
flex-grow: 1;
font-family: "Poppins";
font-size: 1rem;
line-height: 1.5;
|}
  ];

module LogBox = {
  let textDoneCss = [%cx {| color: $(Theme.foregroundSubtle) |}];
  let textCurrentCss = [%cx {| color: $(Theme.foreground) |}];

  let iconSuccessCss = [%cx {| color: $(Theme.success) |}];
  let iconCurrentCss = [%cx {| color: $(Theme.primary) |}];

  let hintElementCss = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 1rem;
    overflow-anchor: none;
  |}
  ];

  let makeHintElement_ = (~currentIndex, ~isFinished, ~hintText) => {
    let textCss = isFinished ? textDoneCss : textCurrentCss;
    let iconElement =
      isFinished
        ? <Icons.CheckLine className=iconSuccessCss />
        : <Icons.RingResize className=iconCurrentCss />;

    let key = string_of_int(currentIndex);
    <FramerMotion.span
      key
      className={hintElementCss ++ " " ++ textCss}
      initial={"y": 25, "opacity": 0}
      animate={"y": 0, "opacity": 1.0}>
      iconElement
      {React.string(hintText)}
    </FramerMotion.span>;
  };

  let logBoxCss = [%cx
    {|
    min-height: 12rem;
    display: flex;
    flex-direction: column-reverse;
    background-color: $(Theme.neutral);
    border-color: $(Theme.backgroundSubtle);
    border-radius: 8px;
    border-style: solid;
    border-width: 2px;
    position: relative;
  |}
  ];

  let logBoxViewCss = [%cx
    {|
    position: absolute;
    top: 0;
    right: 0;
    bottom: 0;
    left: 0;

    display: flex;
    flex-direction: column-reverse;
    overflow: scroll;
    padding: 2rem;

    mask-image:
      linear-gradient(
        to bottom,
        rgba(0,0,0,0),
        rgba(0,0,0,1) 2rem,
        rgba(0,0,0,1) calc(100% - 2rem),
        rgba(0,0,0,0));
  |}
  ];

  let logBoxHintsCss = [%cx
    {|
    display: flex;
    flex-direction: column;
    gap: 1rem;
  |}
  ];

  [@react.component]
  let make = (~stage) => {
    let (hints, setHints) =
      React.useState(() =>
        Array.make(Array.length(Stage.hints), React.null)
      );

    React.useEffect1(
      () =>
        switch (stage) {
        | None => None
        | Some(stage) =>
          let currentIndex = Stage.toIndex(stage);
          let isFinished =
            switch (stage) {
            | Finished => true
            | _ => false
            };
          let hintText = Stage.hints[currentIndex];

          let hintElement =
            makeHintElement_(~currentIndex, ~isFinished, ~hintText);

          setHints(previousHints => {
            let currentHints = Array.copy(previousHints);
            currentHints[currentIndex] = hintElement;

            if (currentIndex > 0) {
              let currentIndex = currentIndex - 1;
              let isFinished = true;
              let hintText = Stage.hints[currentIndex];
              let hintElement =
                makeHintElement_(~currentIndex, ~isFinished, ~hintText);
              currentHints[currentIndex] = hintElement;
            };

            currentHints;
          });

          None;
        },
      [|stage|],
    );

    <div className=logBoxCss>
      <div className=logBoxViewCss>
        <div className=logBoxHintsCss> {React.array(hints)} </div>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~onFinish: unit => Js.Promise.t(unit)) => {
  let (stage, generateKeys) = useGenerateKeys();

  let buttonElement =
    switch (stage) {
    | None => React.string("Get Started!")
    | Some(_) => <Icons.RingResize size="1.5rem" />
    };

  let buttonOnClick = _ => {
    ignore(
      {
        let* _ = generateKeys();
        onFinish();
      },
    );
  };

  <>
    <article className=articleCss>
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
    <div className=buttonGroupCss>
      <button onClick=buttonOnClick className=getStartedButtonCss>
        buttonElement
      </button>
      <a className=learnMoreButtonCss> {React.string("Learn More")} </a>
    </div>
    <LogBox stage />
  </>;
};
