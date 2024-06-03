open InboxPageMessagesStyles;
open InboxPageMessagesCardHooks;

module Face = {
  [@react.component]
  let make = (~faceInitial, ~faceAnimate, ~toggleReveal) => {
    let (isMore, setIsMore) = React.useState(() => false);

    let closeOnClick =
      React.useCallback1(_ => toggleReveal(), [|toggleReveal|]);
    let moreOnClick =
      React.useCallback1(
        _ => setIsMore(prevIsMore => !prevIsMore),
        [|setIsMore|],
      );

    let closeOrDelete =
      if (isMore) {
        <button className=cardFaceFailCss>
          <Icons.DeleteBinLine size="1.2rem" />
          <span> {React.string("Delete")} </span>
        </button>;
      } else {
        <button className=cardFaceCloseCss onClick=closeOnClick>
          <Icons.CornerLeftDownLine size="1.2rem" />
          <span> {React.string("Close")} </span>
        </button>;
      };

    let replyOrBlock =
      if (isMore) {
        <button className=cardFaceFailCss>
          <Icons.SpamLine size="1.2rem" />
          <span> {React.string("Block")} </span>
        </button>;
      } else {
        <button className=cardFaceReplyCss>
          <Icons.ChatLine size="1.2rem" />
          <span> {React.string("Reply")} </span>
        </button>;
      };

    let shareOrReport =
      if (isMore) {
        <button className=cardFaceFailCss>
          <Icons.AlarmWarningLine size="1.2rem" />
          <span> {React.string("Report")} </span>
        </button>;
      } else {
        <button className=cardFaceShareCss>
          <Icons.LinkM size="1.2rem" />
          <span> {React.string("Share")} </span>
        </button>;
      };

    <FramerMotion.div
      className=cardFaceCss
      variants=cardVariants
      initial=faceInitial
      animate=faceAnimate>
      <div className=cardFaceContentCss>
        {React.string("example content :3")}
      </div>
      <button className=cardFaceMoreCss onClick=moreOnClick>
        <Icons.MoreLine size="1.2rem" />
      </button>
      closeOrDelete
      replyOrBlock
      shareOrReport
    </FramerMotion.div>;
  };
};

module Back = {
  [@react.component]
  let make = (~backInitial, ~backAnimate, ~toggleReveal) => {
    let onClick = React.useCallback1(_ => toggleReveal(), [|toggleReveal|]);
    <FramerMotion.div
      className=cardBackCss
      variants=cardVariants
      initial=backInitial
      animate=backAnimate
      onClick>
      <Icons.MailLockLine size="8rem" />
    </FramerMotion.div>;
  };
};

[@react.component]
let make = () => {
  let (faceInitial, faceAnimate, backInitial, backAnimate, toggleReveal) =
    useRevealState();

  <div className=cardOuterCss>
    <Face faceInitial faceAnimate toggleReveal />
    <Back backInitial backAnimate toggleReveal />
  </div>;
};
