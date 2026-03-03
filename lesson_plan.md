Lesson Plan Sistem Operasi (14 Pertemuan)

Berbasis: Operating Systems – Three Easy Pieces (OSTEP)
Alur: Virtualization → Concurrency → Persistence

⸻

Minggu 1 — OS Overview & Abstraction

Cakupan:
	•	Peran dan tujuan OS
	•	Abstraction & virtualization concept
	•	OS sebagai resource manager
	•	Overview tiga pilar: CPU, Memory, Storage

Referensi OSTEP:
	•	Overview / Virtualization roadmap
https://pages.cs.wisc.edu/~remzi/OSTEP/

⸻

Minggu 2 — Process, OS Structure, Process API (Tuntas)

Cakupan:
	•	Program vs Process
	•	Process abstraction & address space
	•	Process state & lifecycle
	•	Context switch (konseptual)
	•	PCB concept
	•	PID & process hierarchy
	•	User mode vs kernel mode
	•	Trap & system call mechanism
	•	Process API: fork, exec, wait, exit

Referensi OSTEP:
	•	Abstraction: Processes
https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-intro.pdf
	•	Process API
https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf

⸻

Minggu 3 — CPU Scheduling Dasar

Cakupan:
	•	Scheduling metrics (turnaround, response, waiting)
	•	FIFO
	•	SJF
	•	Trade-off dasar

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf

⸻

Minggu 4 — Scheduling Lanjut

Cakupan:
	•	Round Robin
	•	MLFQ
	•	Fair-share scheduling
	•	Starvation & aging

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched-mlfq.pdf

⸻

Minggu 5 — Memory Virtualization Dasar

Cakupan:
	•	Address space
	•	Base & bounds
	•	Proteksi memori

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/vm-intro.pdf

⸻

Minggu 6 — Paging

Cakupan:
	•	Paging mechanism
	•	Page table
	•	Address translation

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/vm-paging.pdf

⸻

Minggu 7 — TLB & Translation

Cakupan:
	•	TLB
	•	Multi-level page table
	•	Translation performance

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/vm-tlb.pdf

⸻

Minggu 8 — Page Replacement

Cakupan:
	•	Replacement policy
	•	FIFO
	•	LRU
	•	Clock
	•	Local vs global replacement

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys.pdf

⸻

Minggu 9 — Concurrency Introduction

Cakupan:
	•	Thread abstraction
	•	Race condition
	•	Critical section

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf

⸻

Minggu 10 — Locks

Cakupan:
	•	Mutex
	•	Spinlock
	•	Atomic instruction
	•	Lock correctness

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf

⸻

Minggu 11 — Condition Variable & Semaphore

Cakupan:
	•	Condition variables
	•	Semaphore
	•	Producer–consumer pattern

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-sema.pdf

⸻

Minggu 12 — Deadlock

Cakupan:
	•	Empat kondisi deadlock
	•	Resource allocation graph
	•	Detection & avoidance

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-deadlock.pdf

⸻

Minggu 13 — File System Dasar

Cakupan:
	•	Persistence concept
	•	File abstraction
	•	Inode & metadata
	•	Directory structure

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/file-intro.pdf

⸻

Minggu 14 — Crash Consistency & Storage

Cakupan:
	•	Journaling
	•	Write-ahead logging
	•	RAID concept
	•	Integrasi konsep CPU–Memory–Storage

Referensi:
https://pages.cs.wisc.edu/~remzi/OSTEP/file-journaling.pdf
https://pages.cs.wisc.edu/~remzi/OSTEP/file-raid.pdf

⸻

Jika diinginkan, saya bisa tambahkan versi yang sudah dipetakan ke CPMK + indikator asesmen + bobot evaluasi agar siap masuk RPS resmi ITERA.