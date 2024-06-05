module Lexical = {
  type editor;
  type editorState;
  type rootNode;

  [@mel.module "lexical"] external getRoot: unit => rootNode = "$getRoot";

  module Composer = {
    [@mel.module "@lexical/react/LexicalComposer"] [@react.component]
    external make:
      (~initialConfig: 'initialConfig, ~children: React.element) =>
      React.element =
      "LexicalComposer";
  };

  module ContentEditable = {
    [@mel.module "@lexical/react/LexicalContentEditable"] [@react.component]
    external make:
      (~ariaLabelledBy: string=?, ~tabIndex: int=?, ~className: string=?) =>
      React.element =
      "ContentEditable";
  };

  module OnChangePlugin = {
    [@mel.module "@lexical/react/LexicalOnChangePlugin"] [@react.component]
    external make: (~onChange: (editorState, editor) => unit) => React.element =
      "OnChangePlugin";
  };

  module PlainTextPlugin = {
    [@mel.module "@lexical/react/LexicalPlainTextPlugin"] [@react.component]
    external make: (~contentEditable: React.element) => React.element =
      "PlainTextPlugin";
  };

  [@mel.send]
  external readEditorState: (editorState, unit => unit) => unit = "read";
  [@mel.send]
  external getRootNodeTextContent: rootNode => string = "getTextContent";
};

let useIsClient = () => {
  let (isClient, setIsClient) = React.useState(() => false);
  React.useEffect0(() => {
    setIsClient(_ => true);
    None;
  });
  isClient;
};

open WritePageStyles;

[@react.component]
let make = (~ariaLabelledBy, ~setMessage) => {
  let isClient = useIsClient();

  let initialConfig = {
    "namespace": "MessageEditor",
    "theme": Js.Obj.empty,
    "onError": Js.Console.error,
  };

  let onChange = (editorState, _) => {
    Lexical.readEditorState(
      editorState,
      () => {
        let textContent = Lexical.getRootNodeTextContent(Lexical.getRoot());
        setMessage(_ => textContent);
      },
    );
  };

  if (isClient) {
    let contentEditable =
      <Lexical.ContentEditable
        ariaLabelledBy
        tabIndex=0
        className=messageEditableCss
      />;
    <Lexical.Composer initialConfig>
      <Lexical.PlainTextPlugin contentEditable />
      <Lexical.OnChangePlugin onChange />
    </Lexical.Composer>;
  } else {
    <WritePageEditorInitial />;
  };
};
