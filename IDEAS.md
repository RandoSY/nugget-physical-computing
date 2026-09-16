# Nugget Physical Computing — Idea Record

## Problem

Modern computing hides almost everything that makes computation understandable. Learners can write code without seeing state, timing, buses, memory, logic, I/O, or the physical consequences of execution.

## Central idea

Teach computing by **progressive computational transparency**: begin with the smallest observable machine and add abstraction only after the previous layer is understandable.

## Core progression

- one bit / latch / clock;
- timing and visible state;
- simple microcontroller observation and control;
- assembly and registers;
- C/Forth and reusable firmware libraries;
- debuggers and execution observatories;
- connected instruments and laboratory nodes.

The exact processors may change. The progression from visible mechanism to useful abstraction is the durable idea.

## Catch-the-Clock

One timing/reaction experiment becomes a bridge across technological generations. A 555 can provide timing, NAND logic can expose state, a PIC can observe and score, and later AVR/STM32/browser versions can implement the same conceptual problem. The phenomenon remains stable while the implementation changes.

## PIC Black Box / New Nugget

A device can deliberately move between black-box and transparent modes: first use it, then inspect signals and state, then descend into firmware/assembly/debugging. The point is to teach when abstraction is useful and when opening the box produces understanding.

## UNO/MFS starter node

A classic Arduino UNO plus Multi-Function Shield is a deliberately inexpensive entry point. One fixed front panel can support multiple sensors and later become an SDL node over USB or transparent BLE UART.

The important idea is **standalone first, connected second**: local controls and display remain useful without a host; remote supervision extends rather than replaces the instrument.

## 65C02 Nugget

A CPU becomes teachable when execution is visible and reversible. Source, disassembly, registers, buses, memory, I/O, trace, stepping, rewind, assembler behavior, and validation belong in one explanatory environment.

Validated-v6 evidence must remain distinct from later v8 Forth and v9 Studio extensions; conceptual evolution does not automatically transfer validation claims.

## Why it matters

Nugget is a defense against opaque computing. It gives learners a path from physical logic to real software while preserving an inspectable mental model at each step.

## Distinctive contribution

The estate does not treat old/simple processors as nostalgia. They are **observability instruments**: deliberately constrained systems that let a learner see enough of computation to reason causally.

## Representative evidence

- Catch-the-Clock firmware, simulators, teacher tutorial, review material
- nugget-fwlib historical snapshot
- 65C02 validated-v6 simulator and validation records
- 65C02 curriculum, later Forth/Studio lineage, debugger guide, R&G examples
- UNO/MFS universal desktop lab controller v1.2 source and documentation
- historical 555/7400, PIC, AVR, and other observatory material

## Reconstruction path

Rebuild the progression, not necessarily every board: create one observable bit, then a timed state machine, then a programmable observer/controller, then a visible CPU environment, then connect the machine to a real measurement. At every stage require the learner to predict state before execution and explain observed state afterward.
