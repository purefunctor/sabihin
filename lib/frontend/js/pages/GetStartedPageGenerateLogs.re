module Log = {
  type kind =
    | Loading
    | Failure
    | Success;

  type t = {
    kind,
    message: string,
  };

  let equal = (oldProps, nowProps): bool => {
    let oldLog = oldProps##log;
    let nowLog = nowProps##log;
    String.equal(oldLog.message, nowLog.message)
    && (
      switch (oldLog.kind, nowLog.kind) {
      | (Loading, Loading) => true
      | (Failure, Failure) => true
      | (Success, Success) => true
      | _ => false
      }
    );
  };

  let iconLoadingCss = [%cx {| color: $(Theme.primary) |}];
  let iconFailureCss = [%cx {| color: $(Theme.failure) |}];
  let iconSuccessCss = [%cx {| color: $(Theme.success) |}];

  let textLoadingCss = [%cx {| color: $(Theme.foreground10) |}];
  let textFinishedCss = [%cx {| color: $(Theme.foreground11) |}];

  let logCss = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 1rem;
    overflow-anchor: none;
  |}
  ];

  [@react.component]
  let make = (~log: t) => {
    let iconElement =
      switch (log.kind) {
      | Loading => <Icons.RingResize className=iconLoadingCss />
      | Failure => <Icons.CloseLine className=iconFailureCss />
      | Success => <Icons.CheckLine className=iconSuccessCss />
      };
    let textCss =
      switch (log.kind) {
      | Loading => textLoadingCss
      | Failure => textFinishedCss
      | Success => textFinishedCss
      };
    <FramerMotion.span
      initial={"y": 25, "opacity": 0}
      animate={"y": 0, "opacity": 1.0}
      className={logCss ++ " " ++ textCss}>
      iconElement
      {React.string(log.message)}
    </FramerMotion.span>;
  };

  let make = React.memoCustomCompareProps(make, equal);
};

module Logs = {
  let logsCss = [%cx
    {|
    min-height: 12rem;
    background-color: $(Theme.background11) ;
    border-color: $(Theme.background9);
    border-radius: 8px;
    border-style: solid;
    border-width: 2px;
    position: relative;
  |}
  ];

  let logsViewCss = [%cx
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

  let logsListCss = [%cx
    {|
    display: flex;
    flex-direction: column;
    gap: 1rem;
  |}
  ];

  [@react.component]
  let make = (~logs: array(Log.t)) => {
    <div className=logsCss>
      <div className=logsViewCss>
        <div className=logsListCss>
          {logs
           |> Array.mapi((index, log) => {
                let key = string_of_int(index);
                <Log key log />;
              })
           |> React.array}
        </div>
      </div>
    </div>;
  };
};

let useLogStack = () => {
  let logStack = StackHook.useStack();

  let logs = logStack.stack;

  let push = (log: Log.t) => {
    logStack.push(_ => log);
  };

  let replaceKind = (kind: Log.kind) => {
    logStack.replace((_, {message, _}) => {kind, message});
  };

  let clear = logStack.clear;

  (logs, push, replaceKind, clear);
};
