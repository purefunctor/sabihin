module Book = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M3 18.5V5a3 3 0 0 1 3-3h14a1 1 0 0 1 1 1v18a1 1 0 0 1-1 1H6.5A3.5 3.5 0 0 1 3 18.5M19 20v-3H6.5a1.5 1.5 0 0 0 0 3zM5 15.337A3.486 3.486 0 0 1 6.5 15H19V4H6a1 1 0 0 0-1 1z"
      />
    </svg>;
  };
};

module Chat = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M2 8.994A5.99 5.99 0 0 1 8 3h8c3.313 0 6 2.695 6 5.994V21H8c-3.313 0-6-2.695-6-5.994zM14 11v2h2v-2zm-6 0v2h2v-2z"
      />
    </svg>;
  };
};

module ChatHeart = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M6.455 19L2 22.5V4a1 1 0 0 1 1-1h18a1 1 0 0 1 1 1v14a1 1 0 0 1-1 1zM4 18.385L5.763 17H20V5H4zm8.018-3.685l-3.359-3.36a2.25 2.25 0 0 1 3.182-3.182l.177.177l.176-.177a2.25 2.25 0 0 1 3.182 3.182z"
      />
    </svg>;
  };
};

module ChatPrivate = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M12 2c5.523 0 10 4.477 10 10s-4.477 10-10 10a9.956 9.956 0 0 1-4.708-1.175L2 22l1.176-5.29A9.956 9.956 0 0 1 2 12C2 6.477 6.477 2 12 2m0 2a8 8 0 0 0-8 8c0 1.335.326 2.618.94 3.766l.35.654l-.656 2.946l2.948-.654l.653.349A7.955 7.955 0 0 0 12 20a8 8 0 1 0 0-16m0 3a3 3 0 0 1 3 3v1h1v5H8v-5h1v-1a3 3 0 0 1 3-3m2 6h-4v1h4zm-2-4c-.552 0-1 .45-1 1v1h2v-1a1 1 0 0 0-1-1"
      />
    </svg>;
  };
};

module DoorLock = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M4 3h16a1 1 0 0 1 1 1v16a1 1 0 0 1-1 1H4a1 1 0 0 1-1-1V4a1 1 0 0 1 1-1m1 2v14h14V5zm6 7.792a2.5 2.5 0 1 1 2 0V16h-2z"
      />
    </svg>;
  };
};

module GitPr = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M15 5h2a2 2 0 0 1 2 2v8.17a3.001 3.001 0 1 1-2 0V7h-2v3l-4.5-4L15 2zM5 8.83a3.001 3.001 0 1 1 2 0v6.34a3.001 3.001 0 1 1-2 0zM6 7a1 1 0 1 0 0-2a1 1 0 0 0 0 2m0 12a1 1 0 1 0 0-2a1 1 0 0 0 0 2m12 0a1 1 0 1 0 0-2a1 1 0 0 0 0 2"
      />
    </svg>;
  };
};

module OCamlLogo = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 512 512">
      <path
        fill="currentColor"
        d="M0 223.6V97.4c0-35.6 28.9-64.5 64.5-64.5h383c35.6 0 64.5 28.9 64.5 64.5V479H299.5s-14.1-37-16.8-55.2c-.7-4.7.7-9.4-.8-13.8c-1.2-3.8-4-4.2-5.8-7.3c-4.8-8.2-9.8-18.8-10.2-28.8c-.4-9-3.7-17.2-4.2-26.1c-.2-4.3.3-8.8.1-13c-.1-2.1-.3-6.9-1.3-9.9l1.1-2.8c-.5-1 9.6-.6 12.6-.6c5.1.1 9.9.3 15 .6c10.4.5 19.8.4 29.9-1.2c22.5-3.5 32.9-12.7 38.2-16.5c20.7-14.9 30.1-39.3 30.1-39.3c3.4-7.6 3.4-21.2 10.7-27.3c8.6-7.2 23.1-6.7 33-11.1c5.8-2.6 10-4 15.9-2.7c4.4.9 12.3 6 14.1-1.1c-1.5-.9-2-2.7-2.8-3.6c8.2-.8.2-19.7-3.1-23.5c-5-5.8-13.3-8.5-22.1-10.8c-10.5-2.8-20-6-29.9-4.1c-17.3 3.4-16-6.5-26.2-6.5c-12.2 0-33.9.6-37.7 12.5c-1.7 5.5-3.6 5.8-6.6 10c-2.6 3.6.5 6.9-.9 11c-1.3 4.3-3.3 19.3-5.4 24.6c-3.5 8.9-7.6 20-15.3 20c-10.7 1.3-19.2 1.7-27.9-1.5c-5.2-1.9-14-4.9-18.4-6.7c-20.1-8.4-23.4-17.7-23.4-17.7c-2.2-3.6-23.4-47.8-26.5-55.4c-4-9.9-9.6-17.2-13.6-20.9c-6.2-5.6-12-14.2-24.7-11.7c-2.3.4-10.5.8-16.8 6.1c-4.3 3.6-5.6 11-9.6 17.3c-2.3 3.6-6.3 14-10 22.6c-2.6 6-3.8 10.5-6.5 12.7c-2.2 1.7-7.5 6-14.4-1c-2.6-2.7-9-13.9-13.7-22c-13.7-23.8-19.1-34.5-34.8-34.6c-20-.1-21.5 11.2-30.3 27.5c0-.1-8.8 33.1-40.5 56.4m0 146.1V479h99.2s8-30.9 9.8-37.2c1.5-4.8 3.4-8.7 4.2-13.6c.7-4.7-.1-9.2-.5-13.4c-1-10.7 7.8-14.5 12-23.7c3.8-8.3 6-17.7 9.1-26.2c3-8.1 7.7-19.6 15.8-23.7c-1-1.1-16.8-1.6-21-2.1c-13-1.4-31.5-5.3-39.9-7.3c-5.1-1.2-22.1-7.3-25.8-9c-8.7-4-14.4-14.8-21.2-13.7c-4.3.7-8.5 2.2-11.2 6.5c-2.2 3.6-2.9 9.7-4.5 13.8c-1.8 4.8-4.8 9.2-7.5 13.7c-4.7 8.3-15.4 17.3-18.5 26.6m275.9 92.6c-1.9-4-4.3-11.8-5.9-15.3c-1.5-3.2-6.1-11.6-8.4-14.3c-5-5.9-6.2-6.3-7.7-13.7c-2.6-12.9-9.3-36.3-17.3-52.4c-4.1-8.3-11-15.3-17.3-21.3c-5.5-5.3-17.8-14.2-20-13.8c-20.2 4-26.4 23.8-35.9 39.5c-5.3 8.7-10.8 16-15 25.3c-3.8 8.5-3.5 17.9-10 25.2c-6.7 7.5-11.1 15.4-14.4 25.1c-.6 1.8-6.3 32.4-6.3 32.4h164.7l-.3-.7c-2.2-6.3-5.1-13.6-6.2-16"
      />
    </svg>;
  };
};

module LockPassword = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M18 8h2a1 1 0 0 1 1 1v12a1 1 0 0 1-1 1H4a1 1 0 0 1-1-1V9a1 1 0 0 1 1-1h2V7a6 6 0 1 1 12 0zM5 10v10h14V10zm6 4h2v2h-2zm-4 0h2v2H7zm8 0h2v2h-2zm1-6V7a4 4 0 0 0-8 0v1z"
      />
    </svg>;
  };
};

module User = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M4 22a8 8 0 1 1 16 0h-2a6 6 0 0 0-12 0zm8-9c-3.315 0-6-2.685-6-6s2.685-6 6-6s6 2.685 6 6s-2.685 6-6 6m0-2c2.21 0 4-1.79 4-4s-1.79-4-4-4s-4 1.79-4 4s1.79 4 4 4"
      />
    </svg>;
  };
};

module Sparkle = {
  [@react.component]
  let make = (~size=?, ~className=?) => {
    let size = Option.value(size, ~default="1rem");
    let className = Option.value(className, ~default="");
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width=size
      height=size
      viewBox="0 0 24 24">
      <path
        fill="currentColor"
        d="M14 4.438A2.437 2.437 0 0 0 16.438 2h1.125A2.437 2.437 0 0 0 20 4.438v1.125A2.437 2.437 0 0 0 17.563 8h-1.125A2.437 2.437 0 0 0 14 5.563zM1 11a6 6 0 0 0 6-6h2a6 6 0 0 0 6 6v2a6 6 0 0 0-6 6H7a6 6 0 0 0-6-6zm3.876 1A8.038 8.038 0 0 1 8 15.124A8.038 8.038 0 0 1 11.124 12A8.038 8.038 0 0 1 8 8.876A8.038 8.038 0 0 1 4.876 12m12.374 2A3.25 3.25 0 0 1 14 17.25v1.5A3.25 3.25 0 0 1 17.25 22h1.5A3.25 3.25 0 0 1 22 18.75v-1.5A3.25 3.25 0 0 1 18.75 14z"
      />
    </svg>;
  };
};
