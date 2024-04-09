module ActionGrid = {
  module Action = {
    [@react.component]
    let make = (~text, ~iconFn) => {
      <div className="finalize-action">
        <div className="finalize-action-icon color-primary"> {iconFn()} </div>
        <span className="finalize-action-text poppins-regular">
          {React.string(text)}
        </span>
      </div>;
    };
  };

  [@react.component]
  let make = () => {
    <div className="finalize-content">
      <div className="josefin-sans-title finalize-title">
        {React.string("Get Started")}
      </div>
      <div className="finalize-action-grid">
        <Action
          text="Customize Profile"
          iconFn={_ => <Icons.User size="2rem" />}
        />
        <Action
          text="Compose Message"
          iconFn={_ => <Icons.QuillLine size="2rem" />}
        />
        <Action
          text="Share Inbox Link"
          iconFn={_ => <Icons.Link size="2rem" />}
        />
        <Action
          text="Visit Repository"
          iconFn={_ => <Icons.GitRepo size="2rem" />}
        />
      </div>
      <span className="finalize-flavor-text poppins-regular">
        {React.string("Here's some tasks to get started with!")}
      </span>
    </div>;
  };
};

module ActionLoader = {
  [@react.component]
  let make = () => {
    <div className="finalize-loader">
      <Icons.RingResize size="4rem" className="color-primary" />
      <span className="poppins-regular">
        {React.string("Almost there!")}
      </span>
    </div>;
  };
};

module State = {
  let use = (submit: RegisterHooks.Step.submit) => {
    let isSubmitting = React.useRef(false);
    let (isFinished, setIsFinished) = React.useState(() => false);

    let submitInner = () => {
      open Types_js.Defs_bs;
      open Vault.PromiseLet;

      let _ = {
        let* _ = {
          let method_ = Fetch.Post;
          let headers =
            Fetch.HeadersInit.make({"Content-Type": "application/json"});
          let body =
            write_register_keys_payload_t(submit.keysPayload)
            |> Js.Json.stringify
            |> Fetch.BodyInit.make;
          let requestInit =
            Fetch.RequestInit.make(~method_, ~headers, ~body, ());
          Fetch.fetchWithInit("/api/secrets", requestInit);
        };

        setIsFinished(_ => true);

        resolve();
      };
      ();
    };

    React.useEffect0(() => {
      if (!isSubmitting.current) {
        isSubmitting.current = true;
        submitInner();
      };
      None;
    });

    isFinished;
  };
};

[@react.component]
let make = (~submit: RegisterHooks.Step.submit) => {
  let isFinished = State.use(submit);
  isFinished ? <ActionGrid /> : <ActionLoader />;
};
