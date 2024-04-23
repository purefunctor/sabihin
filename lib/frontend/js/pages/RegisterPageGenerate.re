open RegisterPageGenerateHooks;

let containerCss = [%cx
  {|
margin-top: 12rem;
margin-bottom: 12rem;
display: flex;
flex-direction: column;
align-items: center;
justify-content: center;
gap: 1rem;
|}
];

let mainIconCss = [%cx {|
color: $(Theme.primary);
margin-bottom: 2rem;
|}];

let stepSpanCss = [%cx {|
display: flex;
align-items: center;
gap: 0.5rem;
|}];

let stepIconDoneCss = [%cx {|
color: $(Theme.success);
|}];

let stepIconLoadingCss = [%cx {|
color: $(Theme.primary);
|}];

let stepTextDoneCss = [%cx {|
color: $(Theme.foregroundSubtle);
|}];

[@react.component]
let make = (~generate, ~toSubmit) => {
  let steps = useGenerateKeys(~generate, ~toSubmit);
  <div className=containerCss>
    <Icons.RingResize size="3rem" className=mainIconCss />
    {steps
     |> Array.mapi((key, (kind, text)) => {
          let key = string_of_int(key);
          let iconElement =
            switch (kind) {
            | Step.Done =>
              <Icons.CheckLine size="1.25rem" className=stepIconDoneCss />
            | Step.Current =>
              <Icons.RingResize size="1.25rem" className=stepIconLoadingCss />
            | Step.Final =>
              <Icons.CheckLine size="1.25rem" className=stepIconDoneCss />
            };
          let stepTextCss = {
            switch (kind) {
            | Step.Done => Some(stepTextDoneCss)
            | Step.Current => None
            | Step.Final => None
            };
          };
          <FramerMotion.span
            key
            className=stepSpanCss
            initial={"y": 100, "opacity": 0.0}
            animate={"y": 0, "opacity": 1.0}
            transition={"ease": "easeOut"}>
            iconElement
            <span className=?stepTextCss> {React.string(text)} </span>
          </FramerMotion.span>;
        })
     |> React.array}
  </div>;
};
